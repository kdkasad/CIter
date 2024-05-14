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

#ifndef _CITER_REPEAT_H_
#define _CITER_REPEAT_H_

#include "iterator.h"

iterator_t *citer_repeat(void *);

/*
 * Create an iterator that yields a single item.
 *
 * Parameters:
 *   item - The item to yield.
 *
 * The returned iterator must be freed after use with citer_free().
 */
iterator_t *citer_once(void *);

/*
 * Create an empty iterator.
 *
 * The returned iterator must be freed after use with citer_free().
 */
iterator_t *citer_empty(void);

#endif /* _CITER_REPEAT_H_ */
