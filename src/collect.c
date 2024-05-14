/*
 * CIter - C library for lazily-evaluated iterators.
 * Copyright (C) 2024  Kian Kasad <kian@kasad.com>
 *
 * This file is part of CIter.
 *
 * CIter is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free Software
 * Foundation, version 3 of the License.
 *
 * CIter is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with CIter. If not, see <https://www.gnu.org/licenses/>.
 */

#include "collect.h"

#include <stdlib.h>

static citer_llnode_t *citer_collect_into_linked_list_exact(iterator_t *it, citer_llnode_t **tail_out);

void **citer_collect_into_array(iterator_t *it, size_t *len_out) {
    if (citer_is_infinite(it))
        return NULL;

    size_t len;
    /* Amount to increase len by when growing array. Value of 0 means double len
     * instead of incrementing by a fixed value. */
    size_t len_increment = 0;

    /* Compute a reasonable initial length. */
    if (citer_is_finite(it)) {
        if (it->size_bound.lower == 0)
            len = it->size_bound.upper / 2;
        else
            len = it->size_bound.lower;

        /* If the gap between the lower and upper bounds is less than the
         * initial length, just grow to the upper bound instead of doubling the
         * length. */
        if ((it->size_bound.upper - it->size_bound.lower) < len)
            len_increment = it->size_bound.upper - it->size_bound.lower;
    } else if (it->size_bound.lower != 0) {
        len = it->size_bound.lower;
    } else {
        len = 64;
    }

    size_t used = 0;
    void **res = calloc(len, sizeof(*res));
    if (!res)
        return NULL;

    void *item;
    while ((item = citer_next(it))) {
        if (used == len) {
            /* Resize and check for allocation failure. */
            len += len_increment ? len_increment : len;
            void **newres = realloc(res, len * sizeof(*res));
            if (!newres) {
                free(res);
                return NULL;
            }
            res = newres;
        }
        res[used++] = item;
    }

    *len_out = used;
    return res;
}

citer_llnode_t *citer_collect_into_linked_list(iterator_t *it, citer_llnode_t **tail_out) {
    citer_llnode_t *head = NULL;
    citer_llnode_t *tail = NULL;

    /* Use a more efficient approach for exact-sized iterators. */
    if (citer_has_exact_size(it) && (it->size_bound.upper >= 2))
        return citer_collect_into_linked_list_exact(it, tail_out);

    void *item;
    while ((item = citer_next(it))) {
        citer_llnode_t *node = malloc(sizeof(*node));
        if (!node) {
            /* Free all existing nodes and return. */
            while (tail) {
                citer_llnode_t *prev = tail->prev;
                free(tail);
                tail = prev;
            }
            return NULL;
        }

        node->item = item;
        node->prev = tail;
        node->next = NULL;

        if (tail) {
            tail->next = node;
            tail = node;
        } else {
            head = tail = node;
        }
    }

    if (tail_out)
        *tail_out = tail;
    return head;
}

/*
 * Collect into a linked list for an exact-sized iterator.
 * Pre-allocates all nodes in an array to improve efficiency.
 * This function requires that the iterator's length >= 2.
 */
static citer_llnode_t *citer_collect_into_linked_list_exact(iterator_t *it, citer_llnode_t **tail_out) {
    size_t len = it->size_bound.upper;
    citer_llnode_t *arr = malloc(len * sizeof(*arr));

    arr[0].prev = NULL;
    arr[0].next = &arr[1];
    arr[0].item = citer_next(it);
    for (int i = 1; i < len - 1; i++) {
        arr[i].prev = &arr[i - 1];
        arr[i].next = &arr[i + 1];
        arr[i].item = citer_next(it);
    }
    arr[len - 1].prev = &arr[len - 2];
    arr[len - 1].next = NULL;
    arr[len - 1].item = citer_next(it);

    *tail_out = &arr[len - 1];
    return &arr[0];
}
