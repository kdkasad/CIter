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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <citer.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
		fprintf(stderr, "Usage: %s <item> <count>\n", argv[0]);
		return 1;
	}

    unsigned long count;
    if (sscanf(argv[2], "%lu", &count) != 1) {
        fprintf(stderr, "Invalid count: %s\n", argv[1]);
        return 1;
    }

	iterator_t *it = citer_take(citer_repeat(argv[1]), count);

    size_t len;
    char **arr = (char **) citer_collect_into_array(it, &len);
    assert(len == count);

    printf("Generated array: [");
    for (unsigned long i = 0; i < count; i++) {
        printf("%s%s", arr[i], i == count - 1 ? "" : ", ");
    }
    printf("]\n");

	citer_free(it);
    free(arr);

	return 0;
}
