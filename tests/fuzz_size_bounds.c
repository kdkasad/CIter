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

#include <assert.h>
#include <dlfcn.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <citer.h>


#define UNUSED(x) ((void) (x))

#define LIMIT 131072 // 131072 = 128 KiB to B


typedef iterator_t *(*constructor_t)();

struct iterable {
    iterator_t *(*fn)();
    size_t nargs;
};

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


static void interrupt_handler(int signal);
static bool predicate_random(void *item, void *fn_data);
static void *map_noop(void *item, void *fn_data);
static iterator_t *random_chain(size_t maxlen, char **str_out);
static long randomnz(long max);
static unsigned long urandomnz(unsigned long max);


static struct malloc_entry *malloc_entries_head;

static const constructor_t SOURCES[] = {
    citer_empty,
    citer_once,
    citer_repeat,
    citer_over_array,
};
static const size_t N_SOURCES = sizeof(SOURCES) / sizeof(*SOURCES);

static const constructor_t TRANSFORMERS[] = {
    citer_chain,
    citer_chunked,
    citer_enumerate,
    citer_filter,
    citer_flatten,
    citer_inspect,
    citer_map,
    citer_reverse,
    citer_skip,
    citer_skip_while,
    citer_take,
    citer_take_while,
    citer_zip,
};
static const size_t N_TRANSFORMERS = sizeof(TRANSFORMERS) / sizeof(*TRANSFORMERS);

static bool running = true;


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

static void interrupt_handler(int signal) {
    UNUSED(signal);
    running = false;
}

static void *map_noop(void *item, void *fn_data) {
    UNUSED(fn_data);
    return item;
}

static bool predicate_random(void *item, void *fn_data) {
    UNUSED(item);
    UNUSED(fn_data);
    return random() % 2;
}

/*
 * Randomly construct an iterator chain.
 */
static iterator_t *random_chain(size_t maxlen, char **str_out) {
    iterator_t *it;

    size_t r = random();
    constructor_t fn;
    if (maxlen == 1) {
        /* If length is 1, pick a source, which will ensure the chain does not
         * get too long. */
        r %= N_SOURCES;
        fn = SOURCES[r];
    } else {
        /* Otherwise, pick either a source or a transformer. */
        r %= (N_SOURCES + N_TRANSFORMERS);
        if (r < N_SOURCES)
            fn = SOURCES[r];
        else
            fn = TRANSFORMERS[r - N_SOURCES];
    }

    if (fn == citer_empty) {
        asprintf(str_out, "citer_empty()");
        it = citer_empty();
    } else if (fn == citer_once) {
        long num = randomnz(0);
        asprintf(str_out, "citer_once(%ld)", num);
        it = citer_once((void *) num);
    } else if (fn == citer_repeat) {
        long num = randomnz(0);
        asprintf(str_out, "citer_repeat(%ld)", num);
        it = citer_repeat((void *) num);
    } else if (fn == citer_over_array) {
        long *arr;
        size_t len = ((size_t) random()) % (LIMIT / sizeof(*arr));
        arr = malloc(len * sizeof(*arr));
        for (int i = 0; i < len; i++)
            arr[i] = i + 1;
        asprintf(str_out, "citer_over_array([1..%lu], %lu, %lu)", len, sizeof(*arr), len);
        it = citer_over_array(arr, sizeof(*arr), len);
    } else if (fn == citer_chain) {
        char *a_str, *b_str;
        iterator_t *a, *b;
        a = random_chain(maxlen - 1, &a_str);
        b = random_chain(maxlen - 1, &b_str);
        asprintf(str_out, "citer_chain(%s, %s)", a_str, b_str);
        free(a_str);
        free(b_str);
        it = citer_chain(a, b);
    } else if (fn == citer_chunked) {
        char *src_str;
        iterator_t *src = random_chain(maxlen - 1, &src_str);
        size_t chunksize = urandomnz(citer_is_finite(src) ? src->size_bound.upper_infinite : LIMIT / sizeof(void *));
        asprintf(str_out, "citer_chunked(%s, %lu)", src_str, chunksize);
        free(src_str);
        it = citer_chunked(src, chunksize);
    } else if (fn == citer_enumerate) {
        char *src_str;
        iterator_t *src = random_chain(maxlen - 1, &src_str);
        asprintf(str_out, "citer_enumerate(%s)", src_str);
        free(src_str);
        it = citer_enumerate(src);
    } else if (fn == citer_filter) {
        char *src_str;
        iterator_t *src = random_chain(maxlen - 1, &src_str);
        asprintf(str_out, "citer_filter(%s, predicate_random, NULL)", src_str);
        free(src_str);
        it = citer_filter(src, predicate_random, NULL);
    } else if (fn == citer_flatten) {
        /* FIXME: Randomly generate an iterator over iterators so we can use
         * flatten. */
        fprintf(stderr, "Error: flatten not implemented. Using empty instead.\n");
        asprintf(str_out, "citer_empty()");
        it = citer_empty();
    } else if (fn == citer_inspect) {
        char *src_str;
        iterator_t *src = random_chain(maxlen - 1, &src_str);
        asprintf(str_out, "citer_inspect(%s)", src_str);
        free(src_str);
        /* Use map_noop() as the inspector function and ignore the return value. */
        it = citer_inspect(src, (void (*)(void *, void *)) map_noop, NULL);
    } else if (fn == citer_map) {
        char *src_str;
        iterator_t *src = random_chain(maxlen - 1, &src_str);
        asprintf(str_out, "citer_map(%s)", src_str);
        free(src_str);
        it = citer_map(src, map_noop, NULL);
    } else if (fn == citer_reverse) {
        char *src_str;
        iterator_t *src = random_chain(maxlen - 1, &src_str);
        asprintf(str_out, "citer_reverse(%s)", src_str);
        free(src_str);
        it = citer_reverse(src);
    } else if (fn == citer_skip) {
        char *src_str;
        iterator_t *src = random_chain(maxlen - 1, &src_str);
        size_t count = random() % LIMIT;
        asprintf(str_out, "citer_skip(%s, %lu)", src_str, count);
        free(src_str);
        it = citer_skip(src, count);
    } else if (fn == citer_skip_while) {
        char *src_str;
        iterator_t *src = random_chain(maxlen - 1, &src_str);
        asprintf(str_out, "citer_skip_while(%s, predicate_ranom, NULL)", src_str);
        free(src_str);
        it = citer_skip_while(src, predicate_random, NULL);
    } else if (fn == citer_take) {
        char *src_str;
        iterator_t *src = random_chain(maxlen - 1, &src_str);
        size_t count = random() % LIMIT;
        asprintf(str_out, "citer_take(%s, %lu)", src_str, count);
        free(src_str);
        it = citer_take(src, count);
    } else if (fn == citer_take_while) {
        char *src_str;
        iterator_t *src = random_chain(maxlen - 1, &src_str);
        asprintf(str_out, "citer_take_while(%s, predicate_ranom, NULL)", src_str);
        free(src_str);
        it = citer_take_while(src, predicate_random, NULL);
    } else if (fn == citer_zip) {
        char *a_str, *b_str;
        iterator_t *a, *b;
        a = random_chain(maxlen - 1, &a_str);
        b = random_chain(maxlen - 1, &b_str);
        asprintf(str_out, "citer_zip(%s, %s)", a_str, b_str);
        free(a_str);
        free(b_str);
        it = citer_zip(a, b);
    } else {
        fprintf(stderr, "Error: Constructor not implemented (fn = %p)\n", fn);
        asprintf(str_out, "citer_empty()");
        it = citer_empty();
    }

    if (it)
        return it;

    free(*str_out);
    return random_chain(maxlen, str_out);
}

/*
 * Random non-zero
 */
static long randomnz(long modulus) {
    long x = random();
    if (modulus)
        x %= modulus;
    return x == 0 ? 1 : x;
}

/*
 * Unsigned random non-zero
 */
static unsigned long urandomnz(unsigned long modulus) {
    unsigned long x = random();
    if (modulus)
        x %= modulus;
    return x == 0 ? 1 : x;
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [<iterations>]\n", argv[0]);
        return 1;
    }

    size_t max_iterations;
    if ((argc >= 2) && (sscanf(argv[1], "%lu", &max_iterations) != 1)) {
        max_iterations = 250;
    }

    /* Seed RNG */
    srandom(time(NULL));

    /* Register interruption (CTRL-C) handler */
    assert(signal(SIGINT, interrupt_handler) != SIG_ERR);

    size_t iterations = 0;
    while (running && (iterations < max_iterations)) {
        clock_t clock_start;
        char *chain_str;
        iterator_t *it;
        char *error_msg;
        bool error;

        clock_start = clock();
        error = false;

        it = random_chain(randomnz(5), &chain_str);

        /* Print part of log line */
        printf("%lu\t%s\t", iterations + 1, chain_str);
        fflush(stdout);
        free(chain_str);

        if (citer_has_exact_size(it)) {
            /* If the iterator is exact-sized, consume all items and ensure the
             * count matches the bounds. */
            size_t expected = it->size_bound.upper;
            size_t count = 0;
            while (citer_next(it))
                count++;
            if (count != expected) {
                error = true;
                asprintf(&error_msg, "Expected %lu items, got %lu", expected, count);
            }
        } else if (citer_is_infinite(it)) {
            /* We can't verify if it is infinite, so just take an item from each
             * end (if possible) as a weak test. */
            if (!citer_next(it)) {
                error = true;
                asprintf(&error_msg, "Expected item, got none");
            } else if (citer_is_double_ended(it) && !citer_next_back(it)) {
                error = true;
                asprintf(&error_msg, "Expected item (back), got none");
            }
        } else if (citer_is_finite(it)) {
            /* The iterator is finite, so consume all items and ensure the count
             * is between the bounds. */
            size_t lower = it->size_bound.lower;
            size_t upper = it->size_bound.upper;
            size_t count = 0;
            while (citer_next(it))
                count++;
            if ((count < lower) || (count > upper)) {
                error = true;
                asprintf(&error_msg, "Expected range [%lu, %lu], got %lu", lower, upper, count);
            }
        } else {
            /* The iterator has bounds [n, inf), so ensure we can consume n items. */
            size_t lower_bound = it->size_bound.lower;
            size_t count = 0;
            while ((count < lower_bound) && citer_next(it))
                count++;
            if (count < lower_bound) {
                asprintf(&error_msg, "Expected range [%lu, inf), got %lu", lower_bound, count);
            }
        }

        /*
         * Output format (blank means ASCII tab character):
         *
         * iteration chain time_elapsed status error_msg?
         *
         * iteration starts at 1.
         * time_elapsed is in seconds, rounded to 5 decimal places.
         * For the status, E means error and S means success.
         * The error_msg only included if the status is E.
         */
        double time_elapsed = ((double) (clock() - clock_start)) / CLOCKS_PER_SEC;
        printf("%.5lf\t%c",
               time_elapsed,
               error ? 'E' : 'S');
        if (error)
            printf("\t%s", error_msg);
        printf("\n");

        free_all();
        iterations++;
    }

    return 0;
}
