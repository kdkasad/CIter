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

#ifndef _CITER_REVERSE_H_
#define _CITER_REVERSE_H_

#include <stddef.h>

#include "iterator.h"

/*
 * Reverse a double-ended iterator.
 *
 * Returns a new iterator which iterates over the items of the input iterator in
 * reverse.
 *
 * The input iterator must be double-ended. If it is not, this function will
 * return NULL.
 *
 * This function literally swaps the next and next_back function pointers of the
 * input iterator. Unlike other constructors, this function returns the input
 * iterator, not a newly-allocated iterator.
 */
iterator_t *citer_reverse(iterator_t *);

#endif /* _CITER_REVERSE_H_ */
