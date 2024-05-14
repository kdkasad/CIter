/*
 * CIter - C library for lazily-evaluated iterators.
 * Copyright (C) 2024  Kian Kasad <kian@kasad.com>
 *
 * This file is part of CIter.
 *
 * CIter is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free Software
 * Foundation, version 3 of the License.
 *
 * CIter is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with CIter. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _CITER_COLLECT_H_
#define _CITER_COLLECT_H_

#include "iterator.h"

/*
 * Doubly-linked list node structure.
 * Used for collecting items into a linked list.
 */
typedef struct citer_llnode {
    void *item;
    struct citer_llnode *prev;
    struct citer_llnode *next;
} citer_llnode_t;

/*
 * Collect items from an iterator into an array.
 *
 * Only call this function if you know the iterator is finite. If an infinite
 * iterator is passed in, this function will loop forever, likely until it runs
 * out of memory for the array.
 *
 * The second argument to this function is a pointer to a size_t variable that
 * will be set to the length of the returned array.
 *
 * The returned array will be dynamically allocated and must be freed after use.
 *
 * This function will consume all items in the iterator, but will not free the
 * iterator.
 */
void **citer_collect_into_array(iterator_t *, size_t *);

/*
 * Collect items from an iterator into a linked list.
 *
 * Only call this function if you know the iterator is finite. If an infinite
 * iterator is passed in, this function will loop forever, likely until it runs
 * out of memory for the list.
 *
 * The second argument to this function is a pointer to the location at which to
 * store the pointer to the tail node in the returned list. If this argument is
 * NULL, it will not be set.
 *
 * Each node in the returned list will be dynamically allocated and must be
 * freed after use.
 *
 * This function will consume all items in the iterator, but will not free the
 * iterator.
 */
citer_llnode_t *citer_collect_into_linked_list(iterator_t *, citer_llnode_t **);

#endif /* _CITER_COLLECT_H_ */
