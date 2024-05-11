#ifndef _CITER_MAP_H_
#define _CITER_MAP_H_

#include <stddef.h>

#include <citer.h>

iterator_t *citer_map(iterator_t *, void *(*)(void *));

#endif /* _CITER_MAP_H_ */
