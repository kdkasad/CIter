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

#include "chunked.h"

#include <stdlib.h>

#define CEIL_DIV(a, b) (((a) + (b) - 1) / (b))

/* TODO: Once size reporting is implemented, make chunked double-ended. */

typedef struct citer_chunked_data {
    iterator_t *orig;
    size_t chunksize;
} citer_chunked_data_t;

static void *citer_chunked_next(iterator_t *self) {
    citer_chunked_data_t *data = (citer_chunked_data_t *) self->data;

    void *first = citer_next(data->orig);
    if (!first)
        return NULL;

    void **chunk = malloc(data->chunksize * sizeof(*chunk));
    chunk[0] = first;
    for (size_t i = 1; i < data->chunksize; i++) {
        chunk[i] = citer_next(data->orig);
    }
    return chunk;
}

static void citer_chunked_free_data(void *_data) {
    citer_chunked_data_t *data = (citer_chunked_data_t *) _data;
    citer_free(data->orig);
    free(data);
}

iterator_t *citer_chunked(iterator_t *orig, size_t chunksize) {
    /* Chunk size cannot be 0. */
    if (chunksize == 0)
        return NULL;

    citer_chunked_data_t *data = malloc(sizeof(*data));
    *data = (citer_chunked_data_t) {
        .orig = orig,
        .chunksize = chunksize,
    };
    return citer_new(
        data,
        citer_chunked_next,
        NULL,
        citer_chunked_free_data,
        (citer_size_bound_t) {
            .lower = CEIL_DIV(orig->size_bound.lower, chunksize),
            .upper = CEIL_DIV(orig->size_bound.upper, chunksize),
            .lower_infinite = orig->size_bound.lower_infinite,
            .upper_infinite = orig->size_bound.upper_infinite,
        }
    );
}
