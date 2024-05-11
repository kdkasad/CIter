#ifndef _CITER_ALL_H_
#define _CITER_ALL_H_

#include <stdbool.h>
#include <stddef.h>

#include <citer.h>

/*
 * Predicate function type.
 *
 * Accepts an item as the first argument and returns a boolean.
 * The second argument is implementation-defined extra data that can be passed
 * to the predicate.
 */
typedef bool (*citer_predicate_t)(void *item, void *extra_data);

/*
 * Returns true if all elements of the iterator satisfy the predicate.
 */
bool citer_all(iterator_t *, citer_predicate_t, void *);

/*
 * Returns true if any element of the iterator satisfies the predicate.
 */
bool citer_any(iterator_t *, citer_predicate_t, void *);

/*
 * Iterator which filters out elements that do not satisfy the predicate.
 *
 * Parameters:
 * - iterator: The iterator to filter.
 * - predicate: The predicate to apply. Will be called with an item as the
 *              argument.
 * - extra_data: Extra data to pass to the predicate.
 *
 * Returns a new iterator.
 * This new iterator returns all the items of the original iterator for which
 * the predicate returns true.
 *
 * The returned iterator must be freed with citer_free().
 * When this iterator is freed, the original iterator is also freed.
 */
iterator_t *citer_filter(iterator_t *, citer_predicate_t, void *);

#endif /* _CITER_ALL_H_ */
