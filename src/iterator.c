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
#include <string.h>

/*
 * Create a new iterator.
 */
void *citer_new(
	void *data,
	size_t data_size,
	citer_next_fn next,
	citer_next_fn next_back,
	citer_free_data_fn free_data,
	citer_size_bound_t size_bound
) {
	iterator_t *it = malloc(sizeof((*it)));
	*it = (iterator_t) {
		.size_bound = size_bound,
		.data = data,
		.data_size = data_size,
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
	return it->next(it);
}

/*
 * Get the next item from the back of a double-ended iterator.
 */
void *citer_next_back(iterator_t *it) {
	if (citer_is_double_ended(it))
		return it->next_back(it);
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
 * Clone an iterator.
 *
 * This function will return a new iterator which is an exact copy of the
 * original.
 *
 * If the original iterator has a data_size of 0, only the data pointer will be
 * copied to the new iterator. If data_size > 0, the a copy of the data will be
 * made and stored in the new iterator.
 *
 * Note that even though the data is copied, if the data contains a pointer to
 * other data, that data will not be copied. This may affect, for example,
 * citer_filter(), if state is being stored in the extra predicate data. Or if
 * there was an iterator which read data from a file pointer, its clone would
 * still hold the same file pointer, so the two iterators would not yield the
 * same items.
 */
iterator_t *citer_clone(const iterator_t *orig) {
	iterator_t *it = malloc(sizeof(*it));
	memcpy(it, orig, sizeof(*it));
	if (orig->data_size > 0) {
		it->data = malloc(orig->data_size);
		memcpy(it->data, orig->data, orig->data_size);
	}
	return it;
}

/*
 * Count the number of items in an iterator.
 *
 * Only works for finite iterators. Will return SIZE_MAX for iterators which are
 * guaranteed to be finite. Otherwise will result in an infinite loop.
 *
 * This function may consume the iterator, but will not free it.
 */
size_t citer_count(iterator_t *it) {
	if (citer_has_exact_size(it))
		return it->size_bound.upper;
	else if (citer_is_infinite(it))
		/* TODO: Notify caller of error */
		return SIZE_MAX;

	size_t count = 0;
	while (citer_next(it))
		count++;
	return count;
}
