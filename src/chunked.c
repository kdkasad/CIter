#include "chunked.h"

#include <stdlib.h>

typedef struct citer_chunked_data {
    iterator_t *orig;
    size_t chunksize;
} citer_chunked_data_t;

static void *citer_chunked_next(void *_data) {
    citer_chunked_data_t *data = (citer_chunked_data_t *) _data;

    void *first = citer_next(data->orig);
    if (!first)
        return NULL;

    void **chunk = malloc(data->chunksize * sizeof(*chunk));
    chunk[0] = first;
    for (size_t i = 1; i < data->chunksize; i++) {
        chunk[i] = citer_next(data->orig);
    }
    return chunk;
}

static void citer_chunked_free_data(void *_data) {
    citer_chunked_data_t *data = (citer_chunked_data_t *) _data;
    citer_free(data->orig);
    free(data);
}

iterator_t *citer_chunked(iterator_t *orig, size_t chunksize) {
    /* Chunk size cannot be 0. */
    if (chunksize == 0)
        return NULL;

    citer_chunked_data_t *data = malloc(sizeof(*data));
    *data = (citer_chunked_data_t) {
        .orig = orig,
        .chunksize = chunksize,
    };
    iterator_t *it = malloc(sizeof(*it));
    *it = (iterator_t) {
        .data = data,
        .next = citer_chunked_next,
        .free_data = citer_chunked_free_data,
    };
    return it;
}
