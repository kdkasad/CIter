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

static void *citer_repeat_next(void *data) {
	return data;
}

static void citer_repeat_free_data(void *data) {
	return;
}

iterator_t *citer_repeat(void *item) {
	iterator_t *it = malloc(sizeof(*it));
	*it = (iterator_t) {
		.data = item,
		.next = citer_repeat_next,
		.next_back = citer_repeat_next,
		.free_data = citer_repeat_free_data,
	};
	return it;
}

/* We must use a struct so we can change the value of the item pointer from the
 * next(_back)? function. */
typedef struct citer_once_data {
	void *item;
} citer_once_data_t;

static void *citer_once_next(void *_data) {
	citer_once_data_t *data = (citer_once_data_t *) _data;
	void *item = NULL;
	if (data->item) {
		item = data->item;
		data->item = NULL;
	}
	return item;
}

iterator_t *citer_once(void *item) {
	citer_once_data_t *data = malloc(sizeof(*data));
	data->item = item;
	iterator_t *it = malloc(sizeof(*it));
	*it = (iterator_t) {
		.data = data,
		.next = citer_once_next,
		.next_back = citer_once_next,
		.free_data = free,
	};
	return it;
}
