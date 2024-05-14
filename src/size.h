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

#ifndef _CITER_SIZE_H_
#define _CITER_SIZE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * A size bound for an iterator.
 *
 * The size bound is the bound on the number of items the iterator can produce.
 *
 * The bound can be exact or inexact. A bound is considered exact if and only if
 * the upper and lower bounds are equal, and neither is infinite.
 */
typedef struct citer_size_bound {
	size_t lower;
	size_t upper;
	bool lower_infinite: 1;
    bool upper_infinite: 1;
} citer_size_bound_t;

/*
 * A default size bound which is true for all iterators.
 * Bound is [0, inf).
 */
#define CITER_DEFAULT_SIZE_BOUND \
    ((citer_size_bound_t) { \
        .lower = 0, \
        .upper = 0, \
        .lower_infinite = false, \
        .upper_infinite = true, \
    })

/*
 * Check if a size bound is exact.
 *
 * Returns 1 if the bound is exact, 0 otherwise.
 */
#define citer_bound_is_exact(bound) (((bound).lower == (bound).upper) && !(bound).lower_infinite && !(bound).upper_infinite)

/*
 * True if N is greater than the upper limit of this size bound.
 */
#define citer_gt_upper(bound, N) (!(bound).upper_infinite && ((N) > (bound).upper)))

/*
 * True if N is greater than or equal to the upper limit of this size bound.
 */
#define citer_ge_upper(bound, N) (!(bound).upper_infinite && ((N) >= (bound).upper)))

/*
 * True if N is less than the upper limit of this size bound.
 */
#define citer_lt_upper(bound, N) ((bound).upper_infinite || ((N) < (bound).upper)))

/*
 * True if N is less than or equal to the upper limit of this size bound.
 */
#define citer_le_upper(bound, N) ((bound).upper_infinite || ((N) <= (bound).upper)))

/*
 * True if N is greater than the lower limit of this size bound.
 */
#define citer_gt_lower(bound, N) (!(bound).lower_infinite && ((N) > (bound).lower)))

/*
 * True if N is greater than or equal to the lower limit of this size bound.
 */
#define citer_ge_lower(bound, N) (!(bound).lower_infinite && ((N) >= (bound).lower)))

/*
 * True if N is less than the lower limit of this size bound.
 */
#define citer_lt_lower(bound, N) ((bound).lower_infinite || ((N) < (bound).lower)))

/*
 * True if N is less than or equal to the lower limit of this size bound.
 */
#define citer_le_lower(bound, N) ((bound).lower_infinite || ((N) <= (bound).lower)))

/*
 * Subtract N from the lower and upper limits of the size bound.
 */
#define citer_bound_sub(bound, N) \
    do { \
        if (!(bound).lower_infinite) { \
            if ((bound).lower > (N)) \
                (bound).lower -= (N); \
            else \
                (bound).lower = 0; \
        } \
        if (!(bound).upper_infinite) { \
            if ((bound).upper > (N)) \
                (bound).upper -= (N); \
            else \
                (bound).upper = 0; \
        } \
    } while (0)

/*
 * Add N to the lower and upper limits of the size bound.
 */
#define citer_bound_add(bound, N) \
    do { \
        if (!(bound).lower_infinite) { \
            if ((bound).lower > (SIZE_MAX - (N))) \
                (bound).lower_infinite = true; \
            else \
                (bound).lower += (N); \
        } \
        if (!(bound).upper_infinite) { \
            if ((bound).upper > (SIZE_MAX - (N))) \
                (bound).upper_infinite = true; \
            else \
                (bound).upper += (N); \
        } \
    } while (0)

#endif /* _CITER_SIZE_H_ */
