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

void *sum_accumulator(void *data, void *item) {
    return (void *) (((unsigned long) data) + ((unsigned long) item));
}

void *strptr_to_unsigned_long(void *item, void *fn_data) {
    (void) fn_data; /* Mark as unused */
    char *str = *((char **) item);
    unsigned long num;
    if (sscanf(str, "%lu", &num) != 1) {
        fprintf(stderr, "Invalid number: %s\n", str);
        exit(1);
    }
    return (void *) num;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
		fprintf(stderr, "Usage: %s <items...>\n", argv[0]);
		return 1;
	}

	iterator_t *it = citer_over_array(argv + 1, sizeof(*argv), argc - 1);
    it = citer_map(it, strptr_to_unsigned_long, NULL);

    unsigned long sum = (unsigned long) citer_fold(it, sum_accumulator, (void *) 0);
    printf("Sum: %lu\n", sum);

	citer_free(it);
	return 0;
}
