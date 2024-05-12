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

void *deref_int_ptr(void *item) {
    return (void *) (long) (*((int *) item));
}

int cmp(const void *a, const void *b, const void *extra_data) {
    (void) extra_data; /* Prevents unused argument warnings. */

    return ((int) (long) a) - ((int) (long) b);
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

	iterator_t *it1 = citer_map(citer_over_array(int_arr, sizeof(*int_arr), len), deref_int_ptr);
	iterator_t *it2 = citer_map(citer_over_array(int_arr, sizeof(*int_arr), len), deref_int_ptr);

    printf("Min: %d\n", (int) (long) citer_min(it1, (citer_cmp_fn_t) cmp, NULL));
    printf("Max: %d\n", (int) (long) citer_max(it2, (citer_cmp_fn_t) cmp, NULL));

	citer_free(it1);
	citer_free(it2);

	return 0;
}
