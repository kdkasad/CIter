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

static void *citer_take_next(iterator_t *self) {
	citer_take_data_t *data = (citer_take_data_t *) self->data;
	if (data->count) {
		data->count--;
		citer_bound_sub(self->size_bound, 1);
		return citer_next(data->original);
	} else {
		return NULL;
	}
}

static void *citer_take_next_back(iterator_t *self) {
	citer_take_data_t *data = (citer_take_data_t *) self->data;
	/* Double-endedness is only implemented for exact-size sources, so we can
	 * treat the bounds as the exact number of items. */
	while (data->original->size_bound.upper > data->count) {
		/* Skip elements from the back until len == count. */
		citer_next_back(data->original);
	}
	data->count--;
	citer_bound_sub(self->size_bound, 1);
	return citer_next_back(data->original);
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

	citer_size_bound_t size_bound = {
		.lower = count,
		.upper = count,
		.lower_infinite = false,
		.upper_infinite = false
	};
	/* If source length < count, bounds become source length. */
	if (!original->size_bound.lower_infinite && (original->size_bound.lower < count))
		size_bound.lower = original->size_bound.lower;
	if (!original->size_bound.upper_infinite && (original->size_bound.upper < count))
		size_bound.upper = original->size_bound.upper;

	return citer_new(
		data,
		citer_take_next,
        citer_is_double_ended(original) ? citer_take_next_back : NULL,
		citer_take_free_data,
		size_bound
	);
}

static void *citer_skip_next(iterator_t *self) {
	citer_take_data_t *data = (citer_take_data_t *) self->data;
	while (data->count > 0) {
		data->count--;
		citer_next(data->original);
	}
	citer_bound_sub(self->size_bound, 1);
	return citer_next(data->original);
}

/* TODO: Once size reporting is implemented, make this O(1) for fixed-size
 * iterators. */
static void *citer_skip_next_back(iterator_t *self) {
	citer_take_data_t *data = (citer_take_data_t *) self->data;
	/* Skip items from the front, not the back. */
	while (data->count > 0) {
		data->count--;
		citer_next(data->original);
	}
	citer_bound_sub(self->size_bound, 1);
	/* Return the next item from the back. */
	return citer_next_back(data->original);
}

iterator_t *citer_skip(iterator_t *original, size_t count) {
	citer_take_data_t *data = malloc(sizeof(*data));
	*data = (citer_take_data_t) {
		.original = original,
		.count = count,
	};

	citer_size_bound_t size_bound = original->size_bound;
	citer_bound_sub(size_bound, count);

	return citer_new(
		data,
		citer_skip_next,
        citer_is_double_ended(original) ? citer_skip_next_back : NULL,
		citer_take_free_data,
		size_bound
	);
}

void *citer_nth(iterator_t *it, size_t n) {
	while (n) {
		citer_next(it);
		n--;
	}
	return citer_next(it);
}

void *citer_nth_back(iterator_t *it, size_t n) {
	/* TODO: Notify caller of error. */
	if (!citer_is_double_ended(it))
		return NULL;

	while (n) {
		citer_next_back(it);
		n--;
	}
	return citer_next_back(it);
}

typedef struct citer_take_while_data {
	iterator_t *orig;
	citer_predicate_t predicate;
	void *extra_data;
	unsigned char done;
} citer_take_while_data_t;

static void *citer_take_while_next(iterator_t *self) {
	citer_take_while_data_t *data = (citer_take_while_data_t *) self->data;

	if (data->done)
		return NULL;


	void *item = citer_next(data->orig);
	if (data->predicate(item, data->extra_data)) {
		/* Only decrease the upper bound because the lower bound is always 0. */
		self->size_bound.upper--;
		return item;
	} else {
		data->done = 1;
		self->size_bound.upper = 0;
		self->size_bound.upper_infinite = false;
		return NULL;
	}
}

static void citer_take_while_free_data(void *_data) {
	citer_take_while_data_t *data = (citer_take_while_data_t *) _data;
	citer_free(data->orig);
	free(data);
}

iterator_t *citer_take_while(iterator_t *orig, citer_predicate_t predicate, void *extra_data) {
	citer_take_while_data_t *data = malloc(sizeof(*data));
	*data = (citer_take_while_data_t) {
		.orig = orig,
		.predicate = predicate,
		.extra_data = extra_data,
		.done = 0,
	};

	citer_size_bound_t size_bound = orig->size_bound;
	size_bound.lower = 0;
	size_bound.lower_infinite = false;

	return citer_new(
		data,
		citer_take_while_next,
        NULL,
		citer_take_while_free_data,
		size_bound
	);
}

static void *citer_skip_while_next(iterator_t *self) {
	citer_take_while_data_t *data = (citer_take_while_data_t *) self->data;

	/* Only decrease the upper bound because the lower bound is 0. */
	self->size_bound.upper--;

	if (data->done)
		return citer_next(data->orig);

	void *item;
	while ((item = citer_next(data->orig))) {
		if (!data->predicate(item, data->extra_data)) {
			data->done = 1;

			/* Now that we're done skipping, the lower bound is the lower bound
			 * of the source iterator. */
			self->size_bound.lower = data->orig->size_bound.lower;
			self->size_bound.lower_infinite = data->orig->size_bound.lower_infinite;

			return item;
		}
	}
	return NULL;
}

iterator_t *citer_skip_while(iterator_t *orig, citer_predicate_t predicate, void *extra_data) {
	citer_take_while_data_t *data = malloc(sizeof(*data));
	*data = (citer_take_while_data_t) {
		.orig = orig,
		.predicate = predicate,
		.extra_data = extra_data,
		.done = 0,
	};

	citer_size_bound_t size_bound = orig->size_bound;
	size_bound.lower = 0;
	size_bound.lower_infinite = false;

	return citer_new(
		data,
		citer_skip_while_next,
        NULL,
		citer_take_free_data,
		size_bound
	);
}
