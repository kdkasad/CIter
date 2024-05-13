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

#ifndef _CITER_INSPECT_H_
#define _CITER_INSPECT_H_

#include <stddef.h>

#include "iterator.h"

typedef void (*citer_inspect_fn_t)(void *item, void *fn_data);

/*
 * Iterator which can be used to inspect a chain of iterators.
 *
 * Takes a function which will be called on each item of the iterator. The
 * function does not affect the iterator chain. For that, use citer_map().
 *
 * Note: The inspect function will be called even when the item is NULL, so that
 * should be checked before casting and using the item.
 *
 * The third argument is extra data which will be passed to the callback
 * function. It can be any type and is up to the user to utilize.
 *
 * This is mainly useful for printing items at different steps of the iterator
 * chain. See examples/inspect.c for an example.
 */
iterator_t *citer_inspect(iterator_t *, citer_inspect_fn_t, void *);

#endif /* _CITER_INSPECT_H_ */
