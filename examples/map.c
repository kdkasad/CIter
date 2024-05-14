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

static void *map_int_times2(void *_item, void *fn_data) {
	(void) fn_data; /* Mark as unused. */
    int item = *((int *) _item);
    return (void *) (long) (item * 2);
}

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Usage: %s <nums...>\n", argv[0]);
		return 1;
	}

    int len = argc - 1;
    int *int_arr = calloc(len, sizeof(int));
    for (int i = 1; i < argc; i++) {
        int num;
        if (sscanf(argv[i], "%d", &num) != 1) {
            fprintf(stderr, "Invalid number: %s\n", argv[i]);
            return 1;
        }
        int_arr[i - 1] = num;
    }

	iterator_t *it = citer_over_array(int_arr, sizeof(*int_arr), len);
    it = citer_map(it, map_int_times2, NULL);

	int item;
	unsigned long count = 0;
	while ((item = (int) (long) citer_next(it))) {
		printf("Got: %d\n", item);
		count++;
	}
	printf("Count: %lu\n", count);

	citer_free(it);

	return 0;
}
