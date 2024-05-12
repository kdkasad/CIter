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

/*
 * Iterator structure
 *
 * Fields:
 *   data - Opaque data for this iterator_t
 *   next - A method that takes a pointer to this iterator_t's data and returns
 *          the next item
 */
typedef struct iterator_t {
	void *data;
	void *(*next)(void *data);
	void (*free_data)(void *data);
} iterator_t;

/*
 * Get the next item from an iterator.
 */
void *citer_next(iterator_t *);

/*
 * Free an iterator's data
 */
void citer_free_data(iterator_t *);

/*
 * Free a heap-allocated iterator.
 */
void citer_free(iterator_t *);

#endif /* _CITER_ITERATOR_H_ */
