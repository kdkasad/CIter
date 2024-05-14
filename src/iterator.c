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

#include "iterator.h"

#include <stdbool.h>
#include <stdlib.h>

/*
 * Create a new iterator.
 */
void *citer_new(
	void *data,
	citer_next_fn next,
	citer_next_fn next_back,
	citer_free_data_fn free_data,
	citer_size_bound_t size_bound
) {
	iterator_t *it = malloc(sizeof((*it)));
	*it = (iterator_t) {
		.size_bound = size_bound,
		.data = data,
		.next = next,
		.next_back = next_back,
		.free_data = free_data,
	};
	return it;
}

/*
 * Get the next item from an iterator_t.
 */
void *citer_next(iterator_t *it) {
	return it->next(it->data);
}

/*
 * Get the next item from the back of a double-ended iterator.
 */
void *citer_next_back(iterator_t *it) {
	if (it->next_back)
		return it->next_back(it->data);
	else
		/* TODO: Notify caller of error. */
		return NULL;
}

/*
 * Free an iterator's data
 */
void citer_free_data(iterator_t *it) {
	it->free_data(it->data);
	it->data = NULL;
}

/*
 * Free a heap-allocated iterator.
 */
void citer_free(iterator_t *it) {
	citer_free_data(it);
	free(it);
}

/*
 * Count the number of items in an iterator.
 *
 * Only works for finite iterators. Calling this function on an infinite
 * iterator will result in an infinite loop.
 *
 * This function will consume the iterator, but will not free it.
 */
size_t citer_count(iterator_t *it) {
	size_t count = 0;
	while (citer_next(it))
		count++;
	return count;
}
