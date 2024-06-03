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

typedef void *(*citer_map_fn_t)(void *item, void *fn_data);
typedef iterator_t *(*citer_flat_map_fn_t)(void *item, void *fn_data);

/*
 * Accumulator function for citer_fold().
 *
 * The first argument is the data being accumulated. The type of this data is up
 * to the user.
 * The second argument is the current iterator item.
 *
 * Returns the new value of the accumulated data after processing the item.
 */
typedef void *(*citer_accumulator_fn_t)(void *data, void *item);

/*
 * Map each item of an iterator using a given function.
 *
 * Parameters:
 *   orig - The source iterator to map.
 *   fn - The function to apply to each item. This function should take two
 *        arguments: the first is the item to be processed, and the second is
 *        custom data which can be used by the function.
 *   fn_data - Custom data to be passed as the second argument to the mapping
 *             function. This data can be anything (or nothing); it is up to
 *             the user.
 *
 * Returns a new iterator which yields the result of the mapping function for
 * each input item. This iterator must be freed after use using citer_free().
 * Freeing this iterator will free the source iterator as well, but not the
 * fn_data argument.
 */
iterator_t *citer_map(iterator_t *orig, citer_map_fn_t fn, void *fn_data);

#define citer_flat_map(it, fn) citer_flatten(citer_map((it), (fn)))

/*
 * Flatten an iterator over iterators.
 *
 * Takes an iterator whose items are of type (iterator_t *). Returns an iterator
 * which yields the items of the each iterator in the input iterator.
 *
 * The returned iterator is heap-allocated and must be freed using citer_free().
 * Freeing this iterator will free the original iterator as well.
 */
iterator_t *citer_flatten(iterator_t *);

/*
 * Process an iterator by applying an accumulator function to each item.
 *
 * The result of the accumulator function is passed as the first argument the
 * next time it is called.
 *
 * The third argument to citer_fold() is the initial value for the accumulated
 * data, i.e. what should be passed as the first argument to the accumulator on
 * its first call.
 *
 * This function returns the value returned by the last call to the accumulator.
 * The input iterator will be consumed but will not be freed.
 */
void *citer_fold(iterator_t *, citer_accumulator_fn_t, void *);

#endif /* _CITER_MAP_H_ */
