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

#ifndef _CITER_ENUMERATE_H_
#define _CITER_ENUMERATE_H_

#include <stddef.h>

#include "iterator.h"

typedef struct citer_enumerate_item {
    size_t index;
    void *item;
} citer_enumerate_item_t;

iterator_t *citer_enumerate(iterator_t *iter);

#endif /* _CITER_ENUMERATE_H_ */
