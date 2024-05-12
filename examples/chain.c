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
	if (argc <= 3) {
		fprintf(stderr, "Usage: %s <repeat_item1> <repeat_count1> <repeat_item2> <repeat_count2>\n", argv[0]);
		return 1;
	}

	char *repeat_item1 = argv[1];
	char *repeat_item2 = argv[3];
	unsigned long repeat_count1, repeat_count2;

	if (sscanf(argv[2], "%lu", &repeat_count1) != 1) {
		fprintf(stderr, "Error: repeat_count1 must be a non-negative integer.\n");
		return 1;
	}
	if (sscanf(argv[4], "%lu", &repeat_count2) != 1) {
		fprintf(stderr, "Error: repeat_count2 must be a non-negative integer.\n");
		return 1;
	}
	iterator_t *first = citer_take(citer_repeat(repeat_item1), repeat_count1);
	iterator_t *second = citer_take(citer_repeat(repeat_item2), repeat_count2);
	iterator_t *it = citer_chain(first, second);

	char *item;
	unsigned long count = 0;
	while ((item = (char *) citer_next(it))) {
		printf("Got: %s\n", item);
		count++;
	}
	printf("Count: %lu\n", count);

	citer_free(it);

	return 0;
}
