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

#include "size.h"

/* Forward declaration for use in the function typedefs below. */
typedef struct iterator_t iterator_t;

/*
 * Function type for getting the next item from an iterator.
 * Used for both iterator_t::next() and iterator_t::next_back().
 */
typedef void *(*citer_next_fn)(iterator_t *);

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
struct iterator_t {
	citer_size_bound_t size_bound;
	void *data;
	citer_next_fn next;
	citer_next_fn next_back;
	citer_free_data_fn free_data;
};

/*
 * Create a new iterator.
 */
void *citer_new(
	void *data,
	citer_next_fn next,
	citer_next_fn next_back,
	citer_free_data_fn free_data,
	citer_size_bound_t size_bound
);

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
 * Only works for finite iterators. Will return SIZE_MAX for iterators which are
 * guaranteed to be finite. Otherwise will result in an infinite loop.
 *
 * This function may consume the iterator, but will not free it.
 */
size_t citer_count(iterator_t *);

/*
 * Check if an iterator has an exact size.
 * Returns 1 if the iterator has an exact known size and 0 otherwise.
 */
#define citer_has_exact_size(it) (citer_bound_is_exact((it)->size_bound))

/*
 * Whether an iterator is guaranteed to be finite.
 *
 * Returns 1 if and only if neither bound is infinite.
 *
 * It is possible for an iterator to be finite even when this macro returns 0.
 * This macro checks whether an iterator is guaranteed to be finite, not whether
 * it is possible.
 */
#define citer_is_finite(it) (!(it)->size_bound.lower_infinite && !(it)->size_bound.upper_infinite)

/*
 * Whether an iterator is guaranteed to be infinite.
 *
 * Returns 1 if and only if the iterator's lower size bound is infinite.
 *
 * It is possible for an iterator to return an infinite number of items even
 * when this macro returns 0, because this macro checks whether it is guaranteed
 * to be infinite, not whether it can be infinite.
 */
#define citer_is_infinite(it) (!!(it)->size_bound.lower_infinite)

#endif /* _CITER_ITERATOR_H_ */
