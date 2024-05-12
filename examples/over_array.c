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

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Usage: %s <args...>\n", argv[0]);
		return 1;
	}

	iterator_t *it = citer_over_array((void **) argv + 1, sizeof(*argv), argc - 1);

	char **itemptr;
	unsigned long count = 0;
	while ((itemptr = (char **) citer_next(it))) {
		printf("Got: %s\n", *itemptr);
		count++;
	}
	printf("Count: %lu\n", count);

	citer_free(it);

	return 0;
}
