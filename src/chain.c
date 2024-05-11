#include "chain.h"

#include <stdlib.h>

#include <citer.h>

typedef struct citer_chain_data {
	iterator_t *first;
	iterator_t *second;
} citer_chain_data_t;

static void *citer_chain_next(void *_data) {
	citer_chain_data_t *data = (citer_chain_data_t *) _data;;
	void *item;
	if ((item = citer_next(data->first)))
		return item;
	else
		return citer_next(data->second);
}

static void citer_chain_free_data(void **_data_ptr) {
	citer_chain_data_t **data_ptr = (citer_chain_data_t **) _data_ptr;
	citer_free((*data_ptr)->first);
	citer_free((*data_ptr)->second);
	free(*data_ptr);
	*data_ptr = NULL;
}

iterator_t *citer_chain(iterator_t *first, iterator_t *second) {
	citer_chain_data_t *data = malloc(sizeof(*data));
	*data = (citer_chain_data_t) {
		.first = first,
		.second = second,
	};
	iterator_t *it = malloc(sizeof(*it));
	*it = (iterator_t) {
		.data = data,
		.next = citer_chain_next,
		.free_data = citer_chain_free_data,
	};
	return it;
}
