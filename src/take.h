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

#ifndef _CITER_TAKE_H_
#define _CITER_TAKE_H_

#include <stddef.h>

#include "iterator.h"
#include "filters.h"

iterator_t *citer_take(iterator_t *, size_t);
iterator_t *citer_skip(iterator_t *, size_t);
void *citer_nth(iterator_t *, size_t);

/*
 * Get the nth item from the back end of a double-ended iterator.
 *
 * This function requires a double-ended iterator. If the iterator is not
 * double-ended, it will return NULL.
 */
void *citer_nth_back(iterator_t *, size_t);

/*
 * Take elements from the iterator while the predicate is true.
 *
 * Similar to citer_filter(), but stops when the predicate returns false.
 *
 * The returned iterator must be freed with citer_free().
 * When this iterator is freed, the original iterator is also freed.
 */
iterator_t *citer_take_while(iterator_t *, citer_predicate_t, void *);

/*
 * Skip elements from the iterator while the predicate is true.
 *
 * This is like citer_skip(), but uses a predicate to determine when to stop
 * rather than a fixed number of elements.
 *
 * The returned iterator must be freed with citer_free(). When this iterator is
 * freed, the original iterator is also freed.
 */
iterator_t *citer_skip_while(iterator_t *, citer_predicate_t, void *);

#endif /* _CITER_TAKE_H_ */
