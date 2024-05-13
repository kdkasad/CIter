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

typedef struct citer_chunked_data {
    iterator_t *orig;
    size_t chunksize;
} citer_chunked_data_t;

static void *citer_chunked_next(void *_data) {
    citer_chunked_data_t *data = (citer_chunked_data_t *) _data;

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
    iterator_t *it = malloc(sizeof(*it));
    *it = (iterator_t) {
        .data = data,
        .next = citer_chunked_next,
        .next_back = NULL,
        .free_data = citer_chunked_free_data,
    };
    return it;
}
