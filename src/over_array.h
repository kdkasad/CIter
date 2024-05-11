#ifndef _CITER_OVER_ARRAY_H_
#define _CITER_OVER_ARRAY_H_

#include <stddef.h>

#include <citer.h>

/*
 * Iterator over an array.
 *
 * Parameters:
 * - array: pointer to the first element of the array
 * - itemsize: size (in bytes) of each item in the array
 * - num_items: number of items in the array
 *
 * Returns an new iterator. This iterator's items are pointers to the items in
 * the array.
 *
 * The returned iterator must be freed with citer_free() after use.
 * Freeing the iterator does not free the array itself, only the iterator
 * object. If the array itself needs to be freed, that must be done separately.
 */
iterator_t *citer_over_array(void *array, size_t itemsize, size_t num_items);

#endif /* _CITER_OVER_ARRAY_H_ */
