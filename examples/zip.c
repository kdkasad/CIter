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
		fprintf(stderr, "Usage: %s <list_x...> <list_y...>\n", argv[0]);
		return 1;
	}

    int len = argc - 1;
    if (len % 2 != 0) {
        fprintf(stderr, "Error: Both lists must have the same number of items.\n");
        return 1;
    }

	iterator_t *it1 = citer_over_array(argv + 1, sizeof(*argv), len);
    it1 = citer_take(it1, len / 2);
	iterator_t *it2 = citer_over_array(argv + 1, sizeof(*argv), len);
    it2 = citer_skip(it2, len / 2);
    iterator_t *it = citer_zip(it1, it2);

    void *item;
    while ((item = citer_next(it))) {
        citer_pair_t *pair = (citer_pair_t *) item;
        char *x = *((char **) pair->x);
        char *y = *((char **) pair->y);
        printf("Got: (%s, %s)\n", x, y);
    }
    fflush(stdout);

	citer_free(it);

	return 0;
}
