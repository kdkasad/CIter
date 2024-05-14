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

typedef struct citer_chunked_data {
    iterator_t *orig;
    size_t chunksize;
} citer_chunked_data_t;

static void *citer_chunked_next(iterator_t *self) {
    citer_chunked_data_t *data = (citer_chunked_data_t *) self->data;

    void *first = citer_next(data->orig);
    if (!first)
        return NULL;

    citer_bound_sub(self->size_bound, 1);

    void **chunk = malloc(data->chunksize * sizeof(*chunk));
    chunk[0] = first;
    for (size_t i = 1; i < data->chunksize; i++) {
        chunk[i] = citer_next(data->orig);
    }
    return chunk;
}

/*
 * Double-endedness is only implemented for exact-size sources, so we can freely
 * use the source's size_bound as the source length here.
 */
static void *citer_chunked_next_back(iterator_t *self) {
    citer_chunked_data_t *data = (citer_chunked_data_t *) self->data;

    size_t len = data->orig->size_bound.upper;
    if (len == 0)
        return NULL;

    size_t n_in_last = len % data->chunksize;
    if (n_in_last == 0)
        n_in_last = data->chunksize;

    void **chunk = malloc(data->chunksize * sizeof(*chunk));
    for (int i = n_in_last - 1; i >= 0; i--) {
        chunk[i] = citer_next_back(data->orig);
    }
    for (int i = n_in_last; i < data->chunksize; i++) {
        chunk[i] = NULL;
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
        citer_has_exact_size(orig) ? citer_chunked_next_back : NULL,
        citer_chunked_free_data,
        (citer_size_bound_t) {
            .lower = CEIL_DIV(orig->size_bound.lower, chunksize),
            .upper = CEIL_DIV(orig->size_bound.upper, chunksize),
            .lower_infinite = orig->size_bound.lower_infinite,
            .upper_infinite = orig->size_bound.upper_infinite,
        }
    );
}
