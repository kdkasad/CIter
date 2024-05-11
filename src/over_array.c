#include "over_array.h"

#include <stdlib.h>

#include <citer.h>

typedef struct citer_over_array_data {
	void **array;
	size_t len;
	size_t i;
} citer_over_array_data_t;

static void *citer_over_array_next(void *_data) {
	citer_over_array_data_t *data = (citer_over_array_data_t *) _data;
	if (data->i < data->len) {
		return data->array[data->i++];
	} else {
		return NULL;
	}
}

void citer_over_array_free_data(void **_data_ptr) {
	free(*_data_ptr);
	*_data_ptr = NULL;
}

iterator_t *citer_over_array(void *array[], size_t len) {
	citer_over_array_data_t *data = malloc(sizeof(*data));
	*data = (citer_over_array_data_t) {
		.array = array,
		.len = len,
		.i = 0,
	};

	iterator_t *it = malloc(sizeof(*it));
	*it = (iterator_t) {
		.data = data,
		.next = citer_over_array_next,
		.free_data = citer_over_array_free_data,
	};
	return it;
}
