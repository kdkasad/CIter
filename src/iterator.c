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

#include "iterator.h"

#include <stdlib.h>

/*
 * Get the next item from an iterator_t.
 */
void *citer_next(iterator_t *it) {
	return it->next(it->data);
}

/*
 * Free an iterator's data
 */
void citer_free_data(iterator_t *it) {
	it->free_data(it->data);
	it->data = NULL;
}

/*
 * Free a heap-allocated iterator.
 */
void citer_free(iterator_t *it) {
	citer_free_data(it);
	free(it);
}
