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

static void *map_deref(void *item, void *fn_data) {
    (void) fn_data; /* Mark as unused */
    return *((void **) item);
}

int main(int argc, char *argv[]) {
    if (argc <= 2) {
		fprintf(stderr, "Usage: %s <chunksize> <args...>\n", argv[0]);
		return 1;
	}

    unsigned long chunksize;
    if (sscanf(argv[1], "%lu", &chunksize) != 1) {
        fprintf(stderr, "Invalid chunksize: %s\n", argv[1]);
        return 1;
    }

	iterator_t *it = citer_over_array(argv + 2, sizeof(*argv), argc - 2);
    it = citer_map(it, map_deref, NULL);
    it = citer_chunked(it,  chunksize);

    char **chunk;
    while ((chunk = citer_next(it))) {
        printf("Got: [");
        for (unsigned long i = 0; i < chunksize; i++) {
            printf("%s", chunk[i] ? chunk[i] : "NULL");
            if (i < chunksize - 1) {
                printf(", ");
            }
        }
        printf("]\n");
        free(chunk);
    }

	citer_free(it);

	return 0;
}
