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

#include "chain.h"

#include <stdlib.h>

typedef struct citer_chain_data {
	iterator_t *first;
	iterator_t *second;
} citer_chain_data_t;

static void *citer_chain_next(void *_data) {
	citer_chain_data_t *data = (citer_chain_data_t *) _data;;
	void *item;
	if ((item = citer_next(data->first)))
		return item;
	else
		return citer_next(data->second);
}

static void *citer_chain_next_back(void *_data) {
	citer_chain_data_t *data = (citer_chain_data_t *) _data;;
	void *item;
	if ((item = citer_next_back(data->second)))
		return item;
	else
		return citer_next_back(data->first);
}

static void citer_chain_free_data(void *_data) {
	citer_chain_data_t *data = (citer_chain_data_t *) _data;
	citer_free(data->first);
	if (data->first != data->second)
		citer_free(data->second);
	free(data);
}

iterator_t *citer_chain(iterator_t *first, iterator_t *second) {
	citer_chain_data_t *data = malloc(sizeof(*data));
	*data = (citer_chain_data_t) {
		.first = first,
		.second = second,
	};

	citer_size_bound_t size_bound = first->size_bound;
	size_bound.lower_infinite = first->size_bound.lower_infinite | second->size_bound.lower_infinite;
	size_bound.upper_infinite = first->size_bound.upper_infinite | second->size_bound.upper_infinite;
	if (first->size_bound.lower > SIZE_MAX - second->size_bound.lower)
		size_bound.lower_infinite = true;
	else
		size_bound.lower = first->size_bound.lower + second->size_bound.lower;
	if (first->size_bound.upper > SIZE_MAX - second->size_bound.upper)
		size_bound.upper_infinite = true;
	else
		size_bound.upper = first->size_bound.upper + second->size_bound.upper;


	return citer_new(
		data,
		citer_chain_next,
		(citer_is_double_ended(first) && citer_is_double_ended(second)) ? citer_chain_next_back : NULL,
		citer_chain_free_data,
		size_bound
	);
}
