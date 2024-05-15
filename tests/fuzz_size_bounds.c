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

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include <citer.h>


/* For custom malloc(3) wrapper. */
struct malloc_entry {
    void *ptr;
    struct malloc_entry *next;
};


void *malloc(size_t size);
void free(void *);
static void *real_malloc(size_t size);
static void real_free(void *);
static void free_all(void);


static struct malloc_entry *malloc_entries_head;


/*
 * Wrapper around malloc(3) which tracks each allocation, letting us free all
 * allocated items using free_all().
 */
void *malloc(size_t size) {
    void *chunk = real_malloc(size);
    struct malloc_entry *entry = real_malloc(sizeof(*entry));
    entry->ptr = chunk;
    entry->next = malloc_entries_head;
    malloc_entries_head = entry;
    return chunk;
}

/*
 * Wrapper around free(3) which frees the pointer and removes its corresponding
 * entry from the list.
 */
void free(void *ptr) {
    /* Find entry in list */
    struct malloc_entry *entry, **to_entry;
    to_entry = &malloc_entries_head;
    entry = *to_entry;
    while (entry && (entry->ptr != ptr)) {
        to_entry = &entry->next;
        entry = *to_entry;
    }

    /* Remove entry from list */
    if (entry) {
        *to_entry = entry->next;
        real_free(entry);
    }

    /* Call the real free(3) */
    real_free(ptr);
}

/*
 * Finds and calls the real malloc(3) function.
 */
static void *real_malloc(size_t size) {
    static void *(*_real_malloc)(size_t) = NULL;
    if (!_real_malloc)
        _real_malloc = dlsym(RTLD_NEXT, "malloc");
    return _real_malloc(size);
}

/*
 * Finds and calls the real free(3) function.
 */
static void real_free(void *ptr) {
    static void (*_real_free)(void *) = NULL;
    if (!_real_free)
        _real_free = dlsym(RTLD_NEXT, "free");
    return _real_free(ptr);
}

/*
 * Free all entries in the malloc list which were caught by our malloc(3)
 * wrapper.
 */
static void free_all(void) {
    while (malloc_entries_head) {
        struct malloc_entry *next = malloc_entries_head->next;
        real_free(malloc_entries_head->ptr);
        real_free(malloc_entries_head);
        malloc_entries_head = next;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    #pragma message("TODO: Implement fuzzer")

    return 0;
}
