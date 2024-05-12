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

#include <stdio.h>

#include <citer.h>

static void *deref_char_ptr_ptr(void *item) {
    return (void *) (*((char **) item));
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
		fprintf(stderr, "Usage: %s <args...>\n", argv[0]);
		return 1;
	}

	iterator_t *it = citer_over_array((void **) argv + 1, sizeof(*argv), argc - 1);
    it = citer_map(it, deref_char_ptr_ptr);
    it = citer_enumerate(it);

	citer_enumerate_item_t *item;
    while ((item = citer_next(it))) {
        printf("Got: { .index = %lu, .item = %s }\n", item->index, (char *) item->item);
    }

	citer_free(it);

	return 0;
}
