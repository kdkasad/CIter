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

#ifndef _CITER_ZIP_H_
#define _CITER_ZIP_H_

#include "iterator.h"

/*
 * Represents a pair of items.
 */
typedef struct citer_pair {
    void *x;
    void *y;
} citer_pair_t;

/*
 * Zips two iterators together.
 * The resulting iterator will yield pairs of items from the input iterators.
 * The item is a pointer to a citer_pair_t.
 *
 * The same pair structure will be reused each time the iterator is advanced, so
 * pointers to it should not be saved without first being copied.
 *
 * The item from the first iterator is stored in the x field of the pair, and
 * the item from the second iterator is stored in the y field.
 *
 * The zipped iterator will return NULL if either of the input iterators returns
 * NULL.
 *
 * The returned iterator must be freed after use using citer_free().
 * Freeing this iterator also frees the two input iterators.
 */
iterator_t *citer_zip(iterator_t *, iterator_t *);

#endif /* _CITER_ZIP_H_ */
