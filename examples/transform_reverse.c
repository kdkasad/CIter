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
#include <string.h>

#include <citer.h>

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    char *items[] = { "A", "B", "C", "D", "E", "F", "G" };
    size_t len = sizeof(items) / sizeof(*items);
    iterator_t *it1, *it2;

    /* Enumerate reverse */
    it1 = citer_enumerate(citer_over_array(items, sizeof(*items), len));
    it2 = citer_reverse(citer_enumerate(citer_over_array(items, sizeof(*items), len)));
    citer_enumerate_item_t *arr = malloc(len * sizeof(*arr));
    for (int i = 0; i < len; i++) {
        memcpy(arr + i, (citer_enumerate_item_t *) citer_next(it1), sizeof(*arr));
    }
    citer_free(it1);
    for (int i = len - 1; i >= 0; i--) {
        citer_enumerate_item_t *pair = (citer_enumerate_item_t *) citer_next(it2);
        printf("Got: (%lu, %s)\n", pair->index, *((char **) pair->item));
        assert(arr[i].index == pair->index);
        assert(*((char **) arr[i].item) == *((char **) pair->item));
    }
    citer_free(it2);
    free(arr);

    return 0;
}
