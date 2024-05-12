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

#include "take.h"

#include <stdlib.h>

typedef struct citer_take_data {
	iterator_t *original;
	size_t count;
} citer_take_data_t;

static void *citer_take_next(void *_data) {
	citer_take_data_t *data = (citer_take_data_t *) _data;
	if (data->count) {
		data->count--;
		return citer_next(data->original);
	} else {
		return NULL;
	}
}

static void citer_take_free_data(void *_data) {
	citer_take_data_t *data = (citer_take_data_t *) _data;
	citer_free(data->original);
	free(data);
}

iterator_t *citer_take(iterator_t *original, size_t count) {
	citer_take_data_t *data = malloc(sizeof(*data));
	*data = (citer_take_data_t) {
		.original = original,
		.count = count,
	};
	iterator_t *it = malloc(sizeof(*it));
	*it = (iterator_t) {
		.data = data,
		.next = citer_take_next,
		.free_data = citer_take_free_data,
	};
	return it;
}

static void *citer_skip_next(void *_data) {
	citer_take_data_t *data = (citer_take_data_t *) _data;
	while (data->count > 0) {
		data->count--;
		citer_next(data->original);
	}
	return citer_next(data->original);
}

iterator_t *citer_skip(iterator_t *original, size_t count) {
	citer_take_data_t *data = malloc(sizeof(*data));
	*data = (citer_take_data_t) {
		.original = original,
		.count = count,
	};
	iterator_t *it = malloc(sizeof(*it));
	*it = (iterator_t) {
		.data = data,
		.next = citer_skip_next,
		.free_data = citer_take_free_data,
	};
	return it;
}

void *citer_nth(iterator_t *it, size_t n) {
	while (n) {
		citer_next(it);
		n--;
	}
	return citer_next(it);
}
