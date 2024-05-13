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

void **citer_collect_into_array(iterator_t *it) {
#define INITIAL_LEN 10

    size_t len = INITIAL_LEN;
    size_t used = 0;
    void **res = calloc(len, sizeof(*res));
    if (!res)
        return NULL;

    void *item;
    while ((item = citer_next(it))) {
        if (used == len) {
            /* Resize and check for allocation failure. */
            len *= 2;
            void **newres = realloc(res, len * sizeof(*res));
            if (!newres) {
                free(res);
                return NULL;
            }
            res = newres;
        }
        res[used++] = item;
    }

    return res;

#undef INITIAL_LEN
}

citer_llnode_t *citer_collect_into_linked_list(iterator_t *it) {
    citer_llnode_t *head = NULL;
    citer_llnode_t *tail = NULL;

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

    return head;
}
