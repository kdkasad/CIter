#include "take.h"

#include <stdlib.h>

#include <citer.h>

typedef struct citer_take_data {
	iterator_t *original;
	size_t count;
} citer_take_data_t;

static void *citer_take_next(void *_data) {
	citer_take_data_t *data = (citer_take_data_t *) _data;
	if (data->count) {
		data->count--;
		return citer_next(data->original);
	} else {
		return NULL;
	}
}

static void citer_take_free_data(void **_data_ptr) {
	citer_take_data_t **data_ptr = (citer_take_data_t **) _data_ptr;
	citer_free((*data_ptr)->original);
	free(*data_ptr);
	*data_ptr = NULL;
}

iterator_t *citer_take(iterator_t *original, size_t count) {
	citer_take_data_t *data = malloc(sizeof(*data));
	*data = (citer_take_data_t) {
		.original = original,
		.count = count,
	};
	iterator_t *it = malloc(sizeof(*it));
	*it = (iterator_t) {
		.data = data,
		.next = citer_take_next,
		.free_data = citer_take_free_data,
	};
	return it;
}
