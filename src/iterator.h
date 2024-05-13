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

#ifndef _CITER_ITERATOR_H_
#define _CITER_ITERATOR_H_

#include <stdbool.h>
#include <stddef.h>

/*
 * Function type for getting the next item from an iterator.
 * Used for both iterator_t::next() and iterator_t::next_back().
 */
typedef void *(*citer_next_fn)(void *);

/*
 * Function type for freeing an iterator's data.
 * Used for iterator_t::free_data().
 */
typedef void (*citer_free_data_fn)(void *);

/*
 * Iterator structure
 *
 * Fields:
 *   data - Opaque data for this iterator_t
 *   next - A method that takes a pointer to this iterator_t's data and returns
 *          the next item.
 *   next_back - A method that takes a pointer to this iterator_t's data and
 *               returns the next item from the back of the iterator. This
 *               field is NULL for iterators which are not double-ended.
 *   free_data - A method that takes a pointer to this iterator_t's data and
 *               frees (de-allocates) said data.
 */
typedef struct iterator_t {
	void *data;
	citer_next_fn next;
	citer_next_fn next_back;
	citer_free_data_fn free_data;
} iterator_t;

/*
 * Get the next item from an iterator.
 */
void *citer_next(iterator_t *);

/*
 * Get the next item from the back of a double-ended iterator.
 */
void *citer_next_back(iterator_t *);

/*
 * Free an iterator's data
 */
void citer_free_data(iterator_t *);

/*
 * Free a heap-allocated iterator.
 */
void citer_free(iterator_t *);

/*
 * Check if an iterator is double-ended.
 *
 * Returns 1 if the iterator is double-ended, 0 otherwise.
 */
#define citer_is_double_ended(it) (!!(it)->next_back)

/*
 * Count the number of items in an iterator.
 *
 * Only works for finite iterators. Calling this function on an infinite
 * iterator will result in an infinite loop.
 *
 * This function will consume the iterator, but will not free it.
 */
size_t citer_count(iterator_t *);

#endif /* _CITER_ITERATOR_H_ */
