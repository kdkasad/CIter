#include "enumerate.h"

#include <stdlib.h>

typedef struct citer_enumerate_data {
    iterator_t *orig;
    size_t index;
    citer_enumerate_item_t itemspace;
} citer_enumerate_data_t;

static void *citer_enumerate_next(void *_data) {
    citer_enumerate_data_t *data = _data;
    void *next = citer_next(data->orig);
    if (!next)
        return NULL;
    data->itemspace = (citer_enumerate_item_t) {
        .index = data->index++,
        .item = next,
    };
    return &data->itemspace;
}

static void citer_enumerate_free_data(void *_data) {
    citer_enumerate_data_t *data = _data;
    citer_free(data->orig);
    free(data);
}

iterator_t *citer_enumerate(iterator_t *orig) {
    citer_enumerate_data_t *data = malloc(sizeof(*data));
    *data = (citer_enumerate_data_t) {
        .orig = orig,
        .index = 0,
    };
    iterator_t *it = malloc(sizeof(*it));
    *it = (iterator_t) {
        .data = data,
        .next = citer_enumerate_next,
        .free_data = citer_enumerate_free_data,
    };
    return it;
}
