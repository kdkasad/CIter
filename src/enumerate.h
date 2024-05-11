#ifndef _CITER_ENUMERATE_H_
#define _CITER_ENUMERATE_H_

#include <stddef.h>

#include "iterator.h"

typedef struct citer_enumerate_item {
    size_t index;
    void *item;
} citer_enumerate_item_t;

iterator_t *citer_enumerate(iterator_t *iter);

#endif /* _CITER_ENUMERATE_H_ */
