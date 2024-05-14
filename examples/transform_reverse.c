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

static void *map_deref(void *item, void *fn_data) {
    (void) fn_data; /* Mark unused. */
    return (void *) *((void **) item);
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }


    /* Enumerate reverse */
    {
        char *items[] = { "A", "B", "C", "D", "E", "F", "G" };
        size_t len = sizeof(items) / sizeof(*items);

        iterator_t *it1 = citer_enumerate(citer_over_array(items, sizeof(*items), len));
        iterator_t *it2 = citer_reverse(citer_enumerate(citer_over_array(items, sizeof(*items), len)));

        citer_enumerate_item_t *arr = malloc(len * sizeof(*arr));
        for (int i = 0; i < len; i++) {
            memcpy(arr + i, (citer_enumerate_item_t *) citer_next(it1), sizeof(*arr));
        }
        citer_free(it1);

        for (int i = len - 1; i >= 0; i--) {
            citer_enumerate_item_t *pair = (citer_enumerate_item_t *) citer_next(it2);
            printf("Got: (%lu, %s)\n", pair->index, *((char **) pair->item));
            assert(!memcmp(arr + i, pair, sizeof(*arr)));
        }

        citer_free(it2);
        free(arr);
        printf("\n");
    }

    /* Chunked reverse */
    {
        char *items[] = { "A", "B", "C", "D", "E", "F", "G", "H" };
        size_t len = sizeof(items) / sizeof(*items);
        size_t chunksize = 3;

start_chunked:;
        iterator_t *it1 = citer_chunked(citer_map(citer_over_array(items, sizeof(*items), len), map_deref, NULL), chunksize);
        iterator_t *it2 = citer_reverse(citer_chunked(citer_map(citer_over_array(items, sizeof(*items), len), map_deref, NULL), chunksize));

        size_t n_chunks;
        char ***arr = (char ***) citer_collect_into_array(it1, &n_chunks);
        citer_free(it1);

        for (int i = n_chunks - 1; i >= 0; i--) {
            char **chunk = (char **) citer_next(it2);
            printf("Got: [");
            for (int j = 0; j < chunksize; j++) {
                printf("%s%s", chunk[j], j == chunksize - 1 ? "" : ", ");
            }
            printf("]\n");
            assert(!memcmp(arr[i], chunk, sizeof(*chunk) * chunksize));
            free(arr[i]);
            free(chunk);
        }

        citer_free(it2);
        free(arr);
        printf("\n");

        if (chunksize == 3) {
            chunksize = 2;
            goto start_chunked;
        }
    }

    return 0;
}
