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

#include "reverse.h"

#include <stdlib.h>

iterator_t *citer_reverse(iterator_t *orig) {
    if (!citer_is_double_ended(orig))
        return NULL;

    /* Since we don't need to store extra data, we can just use the iterator as
     * the data and use the normal next, next_back, and free functions. */
     return citer_new(
        orig,
        (citer_next_fn) citer_next_back,
        (citer_next_fn) citer_next,
        (citer_free_data_fn) citer_free,
        orig->size_bound
     );
}
