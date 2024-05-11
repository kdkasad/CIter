#include "iterator.h"

#include <stdlib.h>

/*
 * Get the next item from an iterator_t.
 */
void *citer_next(iterator_t *it) {
	return it->next(it->data);
}

/*
 * Free an iterator's data
 */
void citer_free_data(iterator_t *it) {
	it->free_data(it->data);
	it->data = NULL;
}

/*
 * Free a heap-allocated iterator.
 */
void citer_free(iterator_t *it) {
	citer_free_data(it);
	free(it);
}
