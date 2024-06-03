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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <citer.h>

static size_t count;

static void collect_print_free_array(iterator_t *it) {
    size_t len;
    char **arr = (char **) citer_collect_into_array(it, &len);
    assert(len == count);

    printf("Generated array: [");
    for (unsigned long i = 0; i < len; i++) {
        printf("%s%s", arr[i], i == len - 1 ? "" : ", ");
    }
    printf("]\n");

    citer_free(it);
    free(arr);
}

static void collect_print_free_linked_list(iterator_t *it) {
    citer_llnode_t *head, *tail;
    size_t len = 0;
    head = citer_collect_into_linked_list(it, &tail);
    printf("Generated linked list: [");
    for (citer_llnode_t *cur = head; cur; cur = cur->next) {
        len++;
        printf("%s%s", (char *) cur->item, cur == tail ? "" : ", ");
    }
    printf("]\n");
    assert(len == count);
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    char *item = "item";
    void (*collect_print_free)(iterator_t *) = collect_print_free_array;
    iterator_t *it;

start:

    /* global */ count = 10;

    puts("Exact-size iterator:");
    it = citer_take(citer_repeat(item), count);
    assert(citer_has_exact_size(it));
    collect_print_free(it);

    puts("Non-exact finite iterator with lower bound 0:");
    it = citer_take(citer_repeat(item), count);
    it->size_bound.lower = 0;
    assert(!citer_has_exact_size(it) && citer_is_finite(it));
    collect_print_free(it);

    /* Close bounds are bounds where the distance between the lower and upper
     * bounds is less than the lower bound. This is because collect_into_array()
     * acts differently in this case. */
    puts("Non-exact finite iterator with close bounds:");
    it = citer_take(citer_repeat(item), count);
    it->size_bound.lower = 8;
    it->size_bound.upper = 12;
    assert(!citer_has_exact_size(it) && citer_is_finite(it));
    collect_print_free(it);

    puts("Non-exact finite iterator with far bounds:");
    it = citer_take(citer_repeat(item), count);
    it->size_bound.lower = 1;
    it->size_bound.upper = 20;
    assert(!citer_has_exact_size(it) && citer_is_finite(it));
    collect_print_free(it);

    puts("Possibly-infinite iterator:");
    it = citer_take(citer_repeat(item), count);
    it->size_bound.lower /= 2;
    it->size_bound.upper_infinite = true;
    assert(!citer_is_finite(it) && !citer_is_infinite(it));
    collect_print_free(it);

    puts("Exact-size iterator with length 1:");
    it = citer_once(item);
    assert(citer_has_exact_size(it));
    /* global */ count = 1;
    collect_print_free(it);

    puts("Empty iterator (exact-size):");
    it = citer_empty();
    count = 0;
    assert(citer_has_exact_size(it));
    collect_print_free(it);

    puts("Empty iterator (possibly-infinite):");
    it = citer_empty();
    it->size_bound.upper_infinite = true;
    count = 0;
    collect_print_free(it);

    puts("Definitely-infinite iterator:");
    it = citer_repeat(item);
    assert(citer_is_infinite(it));
    assert(citer_collect_into_array(it, NULL) == NULL);
    puts("Generated array: NULL");
    citer_free(it);

    if (collect_print_free == collect_print_free_array) {
        collect_print_free = collect_print_free_linked_list;
        goto start;
    }

    return 0;
}
