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

#include "inspect.h"

#include <stdlib.h>

typedef struct citer_inspect_data {
    iterator_t *orig;
    citer_inspect_fn_t fn;
    void *fn_data;
} citer_inspect_data_t;

static void *citer_inspect_next(void *_data) {
    citer_inspect_data_t *data = (citer_inspect_data_t *) _data;
    void *item = citer_next(data->orig);
    data->fn(item, data->fn_data);
    return item;
}

static void *citer_inspect_next_back(void *_data) {
    citer_inspect_data_t *data = (citer_inspect_data_t *) _data;
    void *item = citer_next_back(data->orig);
    data->fn(item, data->fn_data);
    return item;
}

static void citer_inspect_free_data(void *_data) {
    citer_inspect_data_t *data = (citer_inspect_data_t *) _data;
    citer_free(data->orig);
    free(data);
}

iterator_t *citer_inspect(iterator_t *orig, citer_inspect_fn_t fn, void *fn_data) {
    citer_inspect_data_t *data = malloc(sizeof(*data));
    *data = (citer_inspect_data_t) {
        .orig = orig,
        .fn = fn,
        .fn_data = fn_data,
    };
    return citer_new(
        data,
        citer_inspect_next,
        citer_is_double_ended(orig) ? citer_inspect_next_back : NULL,
        citer_inspect_free_data,
        orig->size_bound
    );
}
