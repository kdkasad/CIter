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

#ifndef _CITER_CHUNKED_H_
#define _CITER_CHUNKED_H_

#include <stddef.h>

#include "iterator.h"

/*
 * Iterator over chunks of another iterator's items.
 *
 * Each chunk is returned as an array of items. If the chunksize does not divide
 * the number of items in the source iterator, the last chunk may have some NULL
 * elements.
 *
 * Each item is a heap-allocated array of type (void *[]) and must be freed by
 * the caller.
 *
 * Parameters:
 *   1. The source iterator to chunk.
 *   2. The size of each chunk. Must be greater than 0.
 *
 * Returns a new iterator which must be freed with citer_free().
 * Freeing this iterator also frees the source iterator.
 */
iterator_t *citer_chunked(iterator_t *, size_t);

#endif /* _CITER_CHUNKED_H_ */
