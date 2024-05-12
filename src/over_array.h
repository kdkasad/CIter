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

#ifndef _CITER_OVER_ARRAY_H_
#define _CITER_OVER_ARRAY_H_

#include <stddef.h>

#include "iterator.h"

/*
 * Iterator over an array.
 *
 * Parameters:
 * - array: pointer to the first element of the array
 * - itemsize: size (in bytes) of each item in the array
 * - num_items: number of items in the array
 *
 * Returns an new iterator. This iterator's items are pointers to the items in
 * the array.
 *
 * The returned iterator must be freed with citer_free() after use.
 * Freeing the iterator does not free the array itself, only the iterator
 * object. If the array itself needs to be freed, that must be done separately.
 */
iterator_t *citer_over_array(void *array, size_t itemsize, size_t num_items);

#endif /* _CITER_OVER_ARRAY_H_ */
