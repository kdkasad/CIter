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

#include "filters.h"

#include <stdbool.h>
#include <stdlib.h>

bool citer_all(iterator_t *it, citer_predicate_t predicate, void *extra_data) {
    void *item;
    while ((item = citer_next(it))) {
        if (!predicate(item, extra_data))
            return false;
    }
    return true;
}

bool citer_any(iterator_t *it, citer_predicate_t predicate, void *extra_data) {
    void *item;
    while ((item = citer_next(it))) {
        if (predicate(item, extra_data))
            return true;
    }
    return false;
}

void *citer_max(iterator_t *it, citer_cmp_fn_t cmp, void *extra_data) {
    void *max = NULL;
    void *cur;
    while ((cur = citer_next(it))) {
        if (!max || (cmp(cur, max, extra_data) > 0))
            max = cur;
    }
    return max;
}

void *citer_min(iterator_t *it, citer_cmp_fn_t cmp, void *extra_data) {
    void *min = NULL;
    void *cur;
    while ((cur = citer_next(it))) {
        if (!min || (cmp(cur, min, extra_data) < 0))
            min = cur;
    }
    return min;
}

void *citer_find(iterator_t *it, citer_predicate_t predicate, void *extra_data) {
    for (;;) {
        void *item = citer_next(it);
        if (!item || predicate(item, extra_data))
            return item;
    }
}

typedef struct citer_filter_data {
    iterator_t *orig;
    citer_predicate_t predicate;
    void *predicate_data;
} citer_filter_data_t;

static void *citer_filter_next(void *_data) {
    citer_filter_data_t *data = (citer_filter_data_t *) _data;
    void *item;
    while ((item = citer_next(data->orig))) {
        if (data->predicate(item, data->predicate_data))
            return item;
    }
    return NULL;
}

static void *citer_filter_next_back(void *_data) {
    citer_filter_data_t *data = (citer_filter_data_t *) _data;
    void *item;
    while ((item = citer_next_back(data->orig))) {
        if (data->predicate(item, data->predicate_data))
            return item;
    }
    return NULL;
}

static void citer_filter_free_data(void *_data) {
    citer_filter_data_t *data = (citer_filter_data_t *) _data;
    citer_free(data->orig);
    free(data);
}

iterator_t *citer_filter(iterator_t *orig, citer_predicate_t predicate, void *extra_data) {
    citer_filter_data_t *data = malloc(sizeof(*data));
    *data = (citer_filter_data_t) {
        .orig = orig,
        .predicate = predicate,
        .predicate_data = extra_data,
    };
    return citer_new(
        data,
        citer_filter_next,
        citer_is_double_ended(orig) ? citer_filter_next_back : NULL,
        citer_filter_free_data
    );
}
