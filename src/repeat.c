#include "repeat.h"

#include <stdlib.h>

#include <citer.h>

static void *citer_repeat_next(void *data) {
	return data;
}

static void citer_repeat_free_data(void **data_ptr) {
	return;
}

iterator_t *citer_repeat(void *item) {
	iterator_t *repeat = malloc(sizeof(*repeat));
	*repeat = (iterator_t) {
		.data = item,
		.next = citer_repeat_next,
		.free_data = citer_repeat_free_data,
	};
	return repeat;
}
