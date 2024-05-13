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

#include "over_array.h"

#include <stdlib.h>

typedef struct citer_over_array_data {
	void *array;
	size_t itemsize;
	size_t len;
	size_t i;
} citer_over_array_data_t;

static void *citer_over_array_next(void *_data) {
	citer_over_array_data_t *data = (citer_over_array_data_t *) _data;
	if (data->i < data->len) {
		/* Cast to (char *) so pointer arithmetic is in terms of bytes. */
		return (void *) (((char *) data->array) + (data->i++ * data->itemsize));
	} else {
		return NULL;
	}
}

static void *citer_over_array_next_back(void *_data) {
	citer_over_array_data_t *data = (citer_over_array_data_t *) _data;
	if (data->i < data->len) {
		/* Cast to (char *) so pointer arithmetic is in terms of bytes. */
		return (void *) (((char *) data->array) + (((data->len--) - 1) * data->itemsize));
	} else {
		return NULL;
	}
}

void citer_over_array_free_data(void *_data) {
	free(_data);
}

iterator_t *citer_over_array(void *array, size_t itemsize, size_t len) {
	citer_over_array_data_t *data = malloc(sizeof(*data));
	*data = (citer_over_array_data_t) {
		.array = array,
		.itemsize = itemsize,
		.len = len,
		.i = 0,
	};

	return citer_new(
		data,
		citer_over_array_next,
		citer_over_array_next_back,
		citer_over_array_free_data
	);
}
