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

#include <stdio.h>
#include <stdlib.h>

#include <citer.h>

#define UNUSED(x) ((void) (x))

static void *map_deref(void *item, void *fn_data) {
    (void) fn_data; /* Mark as unused. */
    return *((void **) item);
}

static bool is_even(void *item, void *fn_data) {
    UNUSED(fn_data);
    return ((unsigned long) item) % 2 == 0;
}

static void print_item(void *item, void *fn_data) {
    printf("%s: %lu\n", (const char *) fn_data, (unsigned long) item);
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    unsigned long arr[] = { 1, 2, 3, 4, 5 };
    iterator_t *it;

    /*
     * Double-ended iterators which must be tested:
     * - [x] over_array
     * - [ ] repeat (but we can't test this without take)
     * - [x] chain
     * - [x] filter
     * - [x] map
     * - [x] once
     * - [x] flatten
     * - [x] inspect
     * - [x] reverse
     */

    it = citer_over_array(arr, sizeof(*arr), sizeof(arr) / sizeof(*arr));
    it = citer_map(it, map_deref, NULL);
    it = citer_chain(it, citer_flatten(citer_once(citer_once((void *) 6ul))));
    it = citer_inspect(it, print_item, "Before map");
    it = citer_filter(it, is_even, NULL);
    it = citer_reverse(it);

    void *item;
    while ((item = citer_next(it)))
        printf("Got: %lu\n", (unsigned long) item);

    citer_free(it);

    return 0;
}
