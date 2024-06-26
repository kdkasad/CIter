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

    /* Take reverse */
    {
        unsigned long items[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        size_t len = sizeof(items) / sizeof(*items);

        iterator_t *it = citer_reverse(citer_take(citer_over_array(items, sizeof(*items), len), 5));
        assert(citer_next(it) == &items[4]);
        assert(citer_next_back(it) == &items[0]);
        assert(citer_next(it) == &items[3]);
        assert(citer_next(it) == &items[2]);
        assert(citer_next(it) == &items[1]);
        assert(citer_next(it) == NULL);
        citer_free(it);
    }

    /* Skip reverse */
    {
        unsigned long items[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        size_t len = sizeof(items) / sizeof(*items);

        iterator_t *it = citer_reverse(citer_skip(citer_over_array(items, sizeof(*items), len), 5));
        assert(citer_next(it) == &items[8]);
        assert(citer_next_back(it) == &items[5]);
        assert(citer_next(it) == &items[7]);
        assert(citer_next(it) == &items[6]);
        assert(citer_next(it) == NULL);
        citer_free(it);
    }

    /* Zip reverse */
    puts("Zip reverse:");
    {
        unsigned long items[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        size_t len = sizeof(items) / sizeof(*items);

        iterator_t *s1 = citer_over_array(items, sizeof(*items), len);
        iterator_t *s2 = citer_reverse(citer_over_array(items, sizeof(*items), len));
        iterator_t *it = citer_reverse(citer_zip(s1, s2));

        printf("Size bound: { lower = %lu, upper = %lu }\n", it->size_bound.lower, it->size_bound.upper);

        for (int i = 0; i < len; i++) {
            citer_pair_t *pair = (citer_pair_t *) citer_next(it);
            unsigned long x = *((unsigned long *) pair->x);
            unsigned long y = *((unsigned long *) pair->y);
            printf("Got: (%lu, %lu)\n", x, y);
            assert(x == items[len - i - 1]);
            assert(y == items[i]);
        }
        assert(citer_next(it) == NULL);
        citer_free(it);
    }

    return 0;
}
