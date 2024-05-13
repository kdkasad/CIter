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

#include "map.h"
#include "iterator.h"

#include <stdlib.h>

typedef struct citer_map_data {
    iterator_t *orig;
    void *(*fn)(void *);
} citer_map_data_t;

static void *citer_map_next(void *_data) {
    citer_map_data_t *data = (citer_map_data_t *) _data;
    void *next = citer_next(data->orig);
    return next ? data->fn(next) : NULL;
}

static void citer_map_free_data(void *_data) {
    citer_map_data_t *data = (citer_map_data_t *) _data;
    citer_free(data->orig);
    free(data);
}

iterator_t *citer_map(iterator_t *orig, citer_map_fn_t fn) {
    citer_map_data_t *data = malloc(sizeof(*data));
    *data = (citer_map_data_t) { .orig = orig, .fn = fn };
    iterator_t *it = malloc(sizeof(*it));
    *it = (iterator_t) {
        .data = data,
        .next = citer_map_next,
        .free_data = citer_map_free_data
    };
    return it;
}

iterator_t *citer_flat_map(iterator_t *orig, citer_flat_map_fn_t fn) {
    return citer_flatten(citer_map(orig, (citer_map_fn_t) fn));
}

typedef struct citer_flatten_data {
    iterator_t *orig;
    iterator_t *cur;
} citer_flatten_data_t;

void *citer_flatten_next(void *_data) {
    citer_flatten_data_t *data = (citer_flatten_data_t *) _data;
    void *item = NULL;
    while (!item) {
        if (data->cur) {
            void *item = citer_next(data->cur);
            if (!item) {
                citer_free(data->cur);
                data->cur = NULL;
            }
        }
        if (!data->cur) {
            data->cur = citer_next(data->orig);
        }
    }
    return item;
}

void citer_flatten_free_data(void *_data) {
    citer_flatten_data_t *data = (citer_flatten_data_t *) _data;
    if (data->cur)
        citer_free(data->cur);
    citer_free(data->orig);
    free(data);
}

iterator_t *citer_flatten(iterator_t *orig) {
    citer_flatten_data_t *data = malloc(sizeof(*data));
    *data = (citer_flatten_data_t) {
        .orig = orig,
        .cur = NULL,
    };
    iterator_t *it = malloc(sizeof(*it));
    *it = (iterator_t) {
        .data = data,
        .next = citer_flatten_next,
        .free_data = citer_flatten_free_data,
    };
    return it;
}
