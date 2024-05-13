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
#include <stdlib.h>

#include <citer.h>

void *count_accumulator(void *data, void *item) {
    size_t count = (size_t) data;
    count++;
    return (void *) count;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
		fprintf(stderr, "Usage: %s <items...>\n", argv[0]);
		return 1;
	}

	iterator_t *it1 = citer_over_array(argv + 1, sizeof(*argv), argc - 1);
	iterator_t *it2 = citer_over_array(argv + 1, sizeof(*argv), argc - 1);

    size_t count = citer_count(it1);
    size_t count_fold = (size_t) citer_fold(it2, count_accumulator, (void *) 0);

    printf("Count using citer_count(): %lu\n", count);
    printf("Count using citer_fold():  %lu\n", count_fold);

	citer_free(it1);
	citer_free(it2);

	return 0;
}
