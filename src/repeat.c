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

#include "repeat.h"

#include <stdlib.h>

static void *citer_repeat_next(void *data) {
	return data;
}

static void citer_repeat_free_data(void *data) {
	return;
}

iterator_t *citer_repeat(void *item) {
	iterator_t *repeat = malloc(sizeof(*repeat));
	*repeat = (iterator_t) {
		.data = item,
		.next = citer_repeat_next,
		.free_data = citer_repeat_free_data,
	};
	return repeat;
}
