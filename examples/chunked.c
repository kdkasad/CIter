#include <stdio.h>
#include <stdlib.h>

#include <citer.h>

static void *map_deref(void *item) {
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
    it = citer_map(it, map_deref);
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
