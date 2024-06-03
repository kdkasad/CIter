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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <citer.h>

static void *strptr_to_unsigned_long(void *item, void *fn_data) {
    (void) fn_data; /* Mark unused. */

    char *str = *((char **) item);
    unsigned long num;
    if (sscanf(str, "%lu", &num) != 1) {
        fprintf(stderr, "Invalid number: %s\n", str);
        exit(1);
    }
    return (void *) num;
}

static bool less_than(void *item, void *data) {
    return ((unsigned long) item) < ((unsigned long) data);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
		fprintf(stderr, "Usage: %s <nums...>\n", argv[0]);
		return 1;
	}

	iterator_t *it = citer_over_array(argv + 1, sizeof(*argv), argc - 1);
    it = citer_map(it, strptr_to_unsigned_long, NULL);
    it = citer_skip_while(it, less_than, (void *) 10ul);
    it = citer_take_while(it, less_than, (void *) 50ul);

    void *item;
    while ((item = citer_next(it))) {
        printf("Got: %lu\n", (unsigned long) item);
    }

	citer_free(it);
	return 0;
}
