#ifndef _CITER_TAKE_H_
#define _CITER_TAKE_H_

#include <stddef.h>

#include "iterator.h"

iterator_t *citer_take(iterator_t *, size_t);
iterator_t *citer_skip(iterator_t *, size_t);

#endif /* _CITER_TAKE_H_ */
