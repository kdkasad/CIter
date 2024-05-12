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

#ifndef _CITER_ALL_H_
#define _CITER_ALL_H_

#include <stdbool.h>
#include <stddef.h>

#include "iterator.h"

/*
 * Predicate function type.
 *
 * Accepts an item as the first argument and returns a boolean.
 * The second argument is implementation-defined extra data that can be passed
 * to the predicate.
 */
typedef bool (*citer_predicate_t)(void *item, void *extra_data);

/*
 * Comparison function type.
 *
 * Accepts two items plus an extra data argument.
 * The extra data argument is implementation-defined and can be used to pass
 * additional data to the comparison function.
 *
 * Returns:
 *   < 0 if the first item is less than the second item,
 *   0 if the items are equal,
 *   > 0 if the first item is greater than the second item.
 */
typedef int (*citer_cmp_fn_t)(void *item1, void *item2, void *extra_data);

/*
 * Returns true if all elements of the iterator satisfy the predicate.
 */
bool citer_all(iterator_t *, citer_predicate_t, void *);

/*
 * Returns true if any element of the iterator satisfies the predicate.
 */
bool citer_any(iterator_t *, citer_predicate_t, void *);

/*
 * Returns the minimum item of the iterator.
 *
 * Uses the provided compare function to compare items.
 * See citer_cmp_fn_t for more information.
 *
 * The third argument to this function is the extra data to be passed to the compare function.
 *
 * This function consumes the entire iterator but does not free it.
 */
void *citer_min(iterator_t *it, citer_cmp_fn_t cmp, void *extra_data);

/*
 * Returns the maximum item of the iterator.
 *
 * Uses the provided compare function to compare items.
 * See citer_cmp_fn_t for more information.
 *
 * The third argument to this function is the extra data to be passed to the compare function.
 *
 * This function consumes the entire iterator but does not free it.
 */
void *citer_max(iterator_t *it, citer_cmp_fn_t cmp, void *extra_data);


/*
 * Iterator which filters out elements that do not satisfy the predicate.
 *
 * Parameters:
 * - iterator: The iterator to filter.
 * - predicate: The predicate to apply. Will be called with an item as the
 *              argument.
 * - extra_data: Extra data to pass to the predicate.
 *
 * Returns a new iterator.
 * This new iterator returns all the items of the original iterator for which
 * the predicate returns true.
 *
 * The returned iterator must be freed with citer_free().
 * When this iterator is freed, the original iterator is also freed.
 */
iterator_t *citer_filter(iterator_t *, citer_predicate_t, void *);

#endif /* _CITER_ALL_H_ */
