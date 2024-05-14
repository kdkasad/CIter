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
#include "take.h"

#include <stdlib.h>

/*
 * Note on double-endedness for zip:
 *
 * Zip can only be double-ended if both sources are double-ended and
 * exact-sized. This is a subset of the case where zip is exact-sized. Both
 * sources must be exact-sized because we must know the number of elements to
 * skip.
 */

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

typedef struct citer_zip_data {
	iterator_t *first;
	iterator_t *second;
	citer_pair_t pair;
} citer_zip_data_t;

static inline void set_to_min_bound(citer_size_bound_t *dst, citer_size_bound_t *a, citer_size_bound_t *b);

static void *citer_zip_next(iterator_t *self) {
	citer_zip_data_t *data = (citer_zip_data_t *) self->data;;

	citer_bound_sub(self->size_bound, 1);

	void *x = citer_next(data->first);
	void *y = citer_next(data->second);

	if (!x || !y)
		return NULL;

	data->pair.x = x;
	data->pair.y = y;
	return &data->pair;
}

/*
 * If this is called, we know:
 *  - Both sources are exact-sized
 *  - Both sources are double-ended
 *  - Self is exact-sized
 */
static void *citer_zip_next_back(iterator_t *self) {
	citer_zip_data_t *data = (citer_zip_data_t *) self->data;

	if (self->size_bound.lower == 0)
		return NULL;

	size_t nth_first = data->first->size_bound.upper - self->size_bound.upper;
	size_t nth_second = data->second->size_bound.upper - self->size_bound.upper;

	void *x = citer_nth_back(data->first, nth_first);
	void *y = citer_nth_back(data->second, nth_second);

	data->pair.x = x;
	data->pair.y = y;
	citer_bound_sub(self->size_bound, 1);
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

	citer_size_bound_t size_bound;
	set_to_min_bound(&size_bound, &first->size_bound, &second->size_bound);

	return citer_new(
		data,
		citer_zip_next,
        (citer_has_exact_size(first) && citer_has_exact_size(second)) ? citer_zip_next_back : NULL,
		citer_zip_free_data,
		size_bound
	);
}

/*
 * Set the destination size bound to the minimum of the two input size bounds.
 */
static inline void set_to_min_bound(citer_size_bound_t *dst, citer_size_bound_t *a, citer_size_bound_t *b) {
	if (a->lower_infinite && b->lower_infinite) {
		dst->lower_infinite = true;
	} else {
		dst->lower_infinite = false;
		if (a->lower_infinite)
			dst->lower = b->lower;
		else if (b->lower_infinite)
			dst->lower = a->lower;
		else
			dst->lower = MIN(a->lower, b->lower);
	}
	if (a->upper_infinite && b->upper_infinite) {
		dst->upper_infinite = true;
	} else {
		dst->upper_infinite = false;
		if (a->upper_infinite)
			dst->upper = b->upper;
		else if (b->upper_infinite)
			dst->upper = a->upper;
		else
			dst->upper = MIN(a->upper, b->upper);
	}
}
