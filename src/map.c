#include "map.h"
#include "iterator.h"

#include <stdlib.h>

typedef struct citer_map_data {
    iterator_t *orig;
    void *(*fn)(void *);
} citer_map_data_t;

static void *citer_map_next(void *_data) {
    citer_map_data_t *data = (citer_map_data_t *) _data;
    void *next = citer_next(data->orig);
    return next ? data->fn(next) : NULL;
}

static void citer_map_free_data(void *_data) {
    citer_map_data_t *data = (citer_map_data_t *) _data;
    citer_free(data->orig);
    free(data);
}

iterator_t *citer_map(iterator_t *orig, citer_map_fn_t fn) {
    citer_map_data_t *data = malloc(sizeof(*data));
    *data = (citer_map_data_t) { .orig = orig, .fn = fn };
    iterator_t *it = malloc(sizeof(*it));
    *it = (iterator_t) {
        .data = data,
        .next = citer_map_next,
        .free_data = citer_map_free_data
    };
    return it;
}

typedef struct citer_flat_map_data {
    iterator_t *orig;
    iterator_t *(*fn)(void *);
    iterator_t *cur;
} citer_flat_map_data_t;

static void *citer_flat_map_next(void *_data) {
    citer_flat_map_data_t *data = (citer_flat_map_data_t *) _data;
    for (;;) {
        if (data->cur) {
            void *item = citer_next(data->cur);
            if (item) {
                return item;
            } else {
                citer_free(data->cur);
                data->cur = NULL;
            }
        }
        void *orig_item = citer_next(data->orig);
        if (!orig_item) {
            return NULL;
        }
        data->cur = data->fn(orig_item);
    }
}

static void citer_flat_map_free_data(void *_data) {
    citer_flat_map_data_t *data = (citer_flat_map_data_t *) _data;
    citer_free(data->orig);
    if (data->cur) {
        citer_free(data->cur);
    }
    free(data);
}

iterator_t *citer_flat_map(iterator_t *orig, citer_flat_map_fn_t fn) {
    citer_flat_map_data_t *data = malloc(sizeof(*data));
    *data = (citer_flat_map_data_t) {
        .orig = orig,
        .fn = fn,
        .cur = NULL
    };
    iterator_t *it = malloc(sizeof(*it));
    *it = (iterator_t) {
        .data = data,
        .next = citer_flat_map_next,
        .free_data = citer_flat_map_free_data
    };
    return it;
}
