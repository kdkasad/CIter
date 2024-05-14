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

#include "repeat.h"

#include <stdlib.h>

static void *citer_repeat_next(iterator_t *self) {
	return self->data;
}

static void citer_repeat_free_data(void *data) {
	return;
}

iterator_t *citer_repeat(void *item) {
	citer_size_bound_t size_bound = {
		.lower = 0,
		.upper = 0,
		.lower_infinite = true,
		.upper_infinite = true
	};
	return citer_new(
		item,
		citer_repeat_next,
		citer_repeat_next,
		citer_repeat_free_data,
		size_bound
	);
}

/* We must use a struct so we can change the value of the item pointer from the
 * next(_back)? function. */
typedef struct citer_once_data {
	void *item;
} citer_once_data_t;

static void *citer_once_next(iterator_t *self) {
	citer_once_data_t *data = (citer_once_data_t *) self->data;
	void *item = NULL;
	if (data->item) {
		self->size_bound.lower = 0;
		self->size_bound.upper = 0;
		item = data->item;
		data->item = NULL;
	}
	return item;
}

iterator_t *citer_once(void *item) {
	citer_once_data_t *data = malloc(sizeof(*data));
	data->item = item;

	citer_size_bound_t size_bound = {
		.lower = 1,
		.upper = 1,
		.lower_infinite = false,
		.upper_infinite = false
	};

	return citer_new(
		data,
		citer_once_next,
		citer_once_next,
		free,
		size_bound
	);
}

/*
 * Create an empty iterator.
 *
 * The returned iterator must be freed after use with citer_free().
 */
iterator_t *citer_empty(void) {
	iterator_t *it = citer_repeat(NULL);
	it->size_bound = (citer_size_bound_t) {
		.lower = 0,
		.upper = 0,
		.lower_infinite = false,
		.upper_infinite = false
	};
	return it;
}
