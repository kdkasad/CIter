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

static void *parse_num(void *item, void *fn_data) {
    (void) fn_data; /* Mark as unused. */

    char *str = *((char **) item);
    unsigned long num;
    if (sscanf(str, "%lu", &num) != 1) {
        fprintf(stderr, "Invalid number: %s\n", str);
        exit(1);
    }
    return (void *) num;
}

static void inspect_fn(void *item, void *step) {
    if (!item)
        return;
    unsigned long num = (unsigned long) item;
    char *step_str = (char *) step;
    printf("%s: %lu\n", step_str, num);
}

static void *add(void *item, void *fn_data) {
    unsigned long inc = (unsigned long) fn_data;
    return (void *) (((unsigned long) item) + inc);
}

static void *sum_accumulator(void *item, void *data) {
    return (void *) (((unsigned long) data) + ((unsigned long) item));
}

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Usage: %s <nums...>\n", argv[0]);
		return 1;
	}

    iterator_t *it = citer_over_array(argv + 1, sizeof(*argv), argc - 1);
    it = citer_map(it, parse_num, NULL);
    it = citer_inspect(it, inspect_fn, "Before adding two");
    it = citer_map(it, add, (void *) 2ul);
    it = citer_inspect(it, inspect_fn, "After adding two");
    unsigned long sum = (unsigned long) citer_fold(it, sum_accumulator, (void *) 0);

    printf("Sum: %lu\n", sum);

	citer_free(it);

	return 0;
}
