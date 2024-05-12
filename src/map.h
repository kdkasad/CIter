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

#ifndef _CITER_MAP_H_
#define _CITER_MAP_H_

#include <stddef.h>

#include "iterator.h"

typedef void *(*citer_map_fn_t)(void *);
typedef iterator_t *(*citer_flat_map_fn_t)(void *);

iterator_t *citer_map(iterator_t *, citer_map_fn_t);
iterator_t *citer_flat_map(iterator_t *, citer_flat_map_fn_t);

#endif /* _CITER_MAP_H_ */
