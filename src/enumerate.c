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

#include "enumerate.h"

#include <stdlib.h>

/* TODO: Once length reporting is implemented, make enumerate double-ended. */

typedef struct citer_enumerate_data {
    iterator_t *orig;
    size_t index;
    citer_enumerate_item_t itemspace;
} citer_enumerate_data_t;

static void *citer_enumerate_next(void *_data) {
    citer_enumerate_data_t *data = _data;
    void *next = citer_next(data->orig);
    if (!next)
        return NULL;
    data->itemspace = (citer_enumerate_item_t) {
        .index = data->index++,
        .item = next,
    };
    return &data->itemspace;
}

static void citer_enumerate_free_data(void *_data) {
    citer_enumerate_data_t *data = _data;
    citer_free(data->orig);
    free(data);
}

iterator_t *citer_enumerate(iterator_t *orig) {
    citer_enumerate_data_t *data = malloc(sizeof(*data));
    *data = (citer_enumerate_data_t) {
        .orig = orig,
        .index = 0,
    };
    return citer_new(
        data,
        citer_enumerate_next,
        NULL,
        citer_enumerate_free_data,
        orig->size_bound
    );
}
