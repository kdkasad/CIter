#ifndef _CITER_H_
#define _CITER_H_

/*
 * Iterator structure
 *
 * Fields:
 *   data - Opaque data for this iterator_t
 *   next - A method that takes a pointer to this iterator_t's data and returns
 *          the next item
 */
typedef struct iterator_t {
  void *data;
  void *(*next)(void *data);
  void (*free_data)(void *data);
} iterator_t;

/*
 * Get the next item from an iterator.
 */
void *citer_next(iterator_t *);

/*
 * Free an iterator's data
 */
void citer_free_data(iterator_t *);

/*
 * Free a heap-allocated iterator.
 */
void citer_free(iterator_t *);

#include "chain.h"
#include "map.h"
#include "over_array.h"
#include "repeat.h"
#include "take.h"

#endif /* _CITER_H_ */
