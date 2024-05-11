#include "filters.h"

#include <stdbool.h>
#include <stdlib.h>

bool citer_all(iterator_t *it, citer_predicate_t predicate, void *extra_data) {
    void *item;
    while ((item = citer_next(it))) {
        if (!predicate(item, extra_data))
            return false;
    }
    return true;
}

bool citer_any(iterator_t *it, citer_predicate_t predicate, void *extra_data) {
    void *item;
    while ((item = citer_next(it))) {
        if (predicate(item, extra_data))
            return true;
    }
    return false;
}

typedef struct citer_filter_data {
    iterator_t *orig;
    citer_predicate_t predicate;
    void *predicate_data;
} citer_filter_data_t;

static void *citer_filter_next(void *_data) {
    citer_filter_data_t *data = (citer_filter_data_t *) _data;
    void *item;
    while ((item = citer_next(data->orig))) {
        if (data->predicate(item, data->predicate_data))
            return item;
    }
    return NULL;
}

static void citer_filter_free_data(void *_data) {
    citer_filter_data_t *data = (citer_filter_data_t *) _data;
    citer_free(data->orig);
    free(data);
}

iterator_t *citer_filter(iterator_t *orig, citer_predicate_t predicate, void *extra_data) {
    citer_filter_data_t *data = malloc(sizeof(*data));
    *data = (citer_filter_data_t) {
        .orig = orig,
        .predicate = predicate,
        .predicate_data = extra_data,
    };
    iterator_t *it = malloc(sizeof(*it));
    *it = (iterator_t) {
        .data = data,
        .next = citer_filter_next,
        .free_data = citer_filter_free_data,
    };
    return it;
}
