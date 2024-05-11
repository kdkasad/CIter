#ifndef _CITER_MAP_H_
#define _CITER_MAP_H_

#include <stddef.h>

#include "iterator.h"

typedef void *(*citer_map_fn_t)(void *);
typedef iterator_t *(*citer_flat_map_fn_t)(void *);

iterator_t *citer_map(iterator_t *, citer_map_fn_t);
iterator_t *citer_flat_map(iterator_t *, citer_flat_map_fn_t);

#endif /* _CITER_MAP_H_ */
