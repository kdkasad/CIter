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

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <string> <count>\n", argv[0]);
		return 1;
	}

	char *item = argv[1];

	unsigned long count;
	if (sscanf(argv[2], "%lu", &count) != 1) {
		fprintf(stderr, "Error: count must be a non-negative integer\n");
		return 1;
	}

	iterator_t *take = citer_take(citer_repeat(item), count);

	char *returned;
	unsigned long returned_count = 0;
	while ((returned = (char *) citer_next(take))) {
		printf("Got: %s\n", item);
		returned_count++;
	}
	printf("Count: %lu\n", returned_count);

	citer_free(take);

	return 0;
}
