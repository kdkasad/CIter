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

#include "zip.h"

#include <stdlib.h>

/* TODO: Once size reporting is implemented, make zip double-ended. */

typedef struct citer_zip_data {
	iterator_t *first;
	iterator_t *second;
	citer_pair_t pair;
} citer_zip_data_t;

static void *citer_zip_next(void *_data) {
	citer_zip_data_t *data = (citer_zip_data_t *) _data;;

	void *x = citer_next(data->first);
	void *y = citer_next(data->second);

	if (!x || !y)
		return NULL;

	data->pair.x = x;
	data->pair.y = y;
	return &data->pair;
}

static void citer_zip_free_data(void *_data) {
	citer_zip_data_t *data = (citer_zip_data_t *) _data;
	citer_free(data->first);
	if (data->first != data->second)
		citer_free(data->second);
	free(data);
}

iterator_t *citer_zip(iterator_t *first, iterator_t *second) {
	citer_zip_data_t *data = malloc(sizeof(*data));
	*data = (citer_zip_data_t) {
		.first = first,
		.second = second,
	};

	/* Both limits are the minimum of the two inputs' bounds. */
	citer_size_bound_t size_bound;
	if (first->size_bound.lower_infinite && second->size_bound.lower_infinite) {
		size_bound.lower_infinite = true;
	} else if (first->size_bound.lower_infinite) {
		size_bound.lower_infinite = false;
		size_bound.lower = second->size_bound.lower;
	} else {
		size_bound.lower_infinite = false;
		size_bound.lower = first->size_bound.lower;
	}
	if (first->size_bound.upper_infinite && second->size_bound.upper_infinite) {
		size_bound.upper_infinite = true;
	} else if (first->size_bound.upper_infinite) {
		size_bound.upper_infinite = false;
		size_bound.upper = second->size_bound.upper;
	} else {
		size_bound.upper_infinite = false;
		size_bound.upper = first->size_bound.upper;
	}

	return citer_new(
		data,
		citer_zip_next,
        NULL,
		citer_zip_free_data,
		size_bound
	);
}
