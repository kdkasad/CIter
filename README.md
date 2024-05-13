<!--
 |
 | CIter - C library for lazily-evaluated iterators.
 | Copyright (C) 2024  Kian Kasad <kian@kasad.com>
 |
 | This file is part of CIter.
 |
 | CIter is free software: you can redistribute it and/or modify it under the terms
 | of the GNU Lesser General Public License as published by the Free Software
 | Foundation, version 3 of the License.
 |
 | CIter is distributed in the hope that it will be useful, but WITHOUT ANY
 | WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 | PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 |
 | You should have received a copy of the GNU Lesser General Public License along
 | with CIter. If not, see <https://www.gnu.org/licenses/>.
 |
 +-->

# CIter: Lazily-evaluated iterators for C

Inspired by Rust, this library attempts to implement lazily-evaluated iterators
in C.

## Usage

To use this library, include the header file `citer.h` in your source code and link against either `libciter.a` or `libciter.so`.
The former is a static library, while the latter is dynamically linked.

To link against the dynamic library once it has been installed, add `-lciter` to your compiler/linker flags (usually `LDLIBS`).
To link against the static library, add `-l:libciter.a`, and specify the directory the library is in using `-L/path/to/libraries`.

See [Programming with CIter](#programming-with-citer) for more information on how to use CIter.
Also see the examples in the `examples/` directory for simple demonstrations of CIter's features.

## Getting CIter

Currently, CIter must be built from sources.
Don't worry though; it's easy!

### Building

Simply run `make` in the root directory of the repository.

This will build three notable files: `citer.h`, `libciter.a`, and `libciter.so.0.1.0`.

The examples in the `examples/` directory will also be built.
Those can be run to test CIter's features.

### Installing

To install CIter, run `make install`.
This may require root privileges.

By default, `PREFIX` is set to `/usr/local` in the Makefile.
To install to a different location, run `make PREFIX=/path/to/install/into install`.
`DESTDIR` can also be set, and will be prepended to the install paths.

Running `make install` will:
 - install `citer.h` as `$(PREFIX)/include/citer.h`;
 - install `libciter.a` as `$(PREFIX)/lib/libciter.a`;
 - install `libciter.so.0.1.0` as `$(PREFIX)/lib/libciter.so.0.1.0`;
 - create symbolic links `libciter.so` and `libciter.so.0` to `libciter.so.0.1.0` in `$(PREFIX)/lib`.

### Uninstalling

To uninstall CIter, run `make uninstall`.
Just like when installing, set the `PREFIX` accordingly.

## Programming with CIter

CIter provides a set of functions and types to work with iterators.
An iterator is an object which repeatedly returns items.

### Items and types

An "item" in this case is opaque data and is handled as a `void *`.
This can be cast to the appropriate type when needed.
An iterator over integers could use pointers to integers, casting each `void *` to an `int *`.
Alternatively, the `void *` could be cast to an `int` directly, since `sizeof(int) <= sizeof(void *)`.

It is up to the user to keep track of data types and cast appropriately.
If you don't like this, try Rust.

### Using iterators

Iterators are created using the `citer_<iterator>(...)` functions.
These functions take parameters as necessary and return a pointer to a heap-allocated iterator.

To free an iterator, call `citer_free(it)`.
`citer_free(it)` will not free any memory which was not allocated by `citer_*()` functions.
This means that if you pass heap-allocated memory to iterator constructor functions, you must free it yourself.

Calling `citer_next(it)` will return the next item in the iterator, or `NULL` if the iterator is exhausted.
If the iterator is double-ended, `citer_next_back(it)` will return the next item from the back of the iterator.

#### Double-ended iterators

Some iterators can iterate both forwards and backwards.
These are called double-ended iterators.

The table of iterators below shows which iterators are double-ended.
To check at runtime if an iterator is double-ended, use `citer_is_double_ended(it)`.

Double-ended iterators should only return each item once.
In other words, `citer_next(it)` should not return the same item as `citer_next_back(it)`.
See the example below for a demonstration.

<details>
<summary>Example</summary>

Here is an example using an iterator over a small array:

    #include <assert.h>
    #include <stdio.h>
    #include <citer.h>

    int main() {
        int arr[] = { 1, 2, 3, 4, 5 };
        iterator_t *it = citer_over_array(arr, sizeof(int), 5);

        assert(citer_next_back(it) == &arr[4]);
        assert(citer_next(it) == &arr[0]);
        assert(citer_next(it) == &arr[1]);
        assert(citer_next(it) == &arr[2]);
        assert(citer_next(it) == &arr[3]);
        assert(citer_next(it) == NULL);

        return 0;
    }

As you can see, the iterator is exhausted after returning the 4th item,
because the 5th was already returned by a call to `citer_next_back()`.

</details>

### Implementing your own iterators

An iterator is very easy to implement.
Take a look in any of the files in the `src/` directory to see how they are implemented.
`repeat.c` is especially simple, and `map.c` is a good example of a very powerful iterator
which still doesn't require much code.

The `iterator_t` type is defined as follows:
```c
typedef void *(*citer_next_fn)(void *);
typedef void (*citer_free_data_fn)(void *);

typedef struct iterator_t {
    void *data;
	citer_next_fn next;
	citer_next_fn next_back;
	citer_free_data_fn free_data;
} iterator_t;
```

The `data` field can be anything you want.
It is passed to the `next` and `next_back` functions but is not used anywhere else (except `free_data()`).

The `next` function should return the next item of this iterator, or `NULL` if the iterator is exhausted.

The `next_back` function is the same as `next`,
but should return the next item from the back (i.e. end) of the iterator.
This is only implemented for double-ended iterators.
For single-ended iterators, the `next_back` field should be set to `NULL`.

The `free_data` function should deallocate any heap-allocated memory in the `data` field.
If `data` is a pointer to a struct, and that struct was heap-allocated when the iterator was created,
`free_data` should free that struct.

It is recommended to create a function to create an iterator,
which heap-allocates the `iterator_t` struct and sets the fields appropriately.

## List of iterators and functions

All iterators and functions below are prefixed with the name `citer_` to avoid clobbering the global namespace.

### Iterators

| Iterator   | Double-ended | Description                                                                                                         |
| ---        | --- | ---                                                                                                                 |
| chain      | I | Chains two iterators. Iterates over all items of the first, then all items of the second.                           |
| chunked    | N | Iterates over N-item chunks of an iterator at a time.                                                               |
| enumerate  | N | Enumerates the items of an iterator. Each new item is a `citer_enumerate_item_t` containing the index and the item. |
| filter     | I | Filters items of an iterator using a predicate function.                                                            |
| flat_map   | I | Maps each item of an iterator to an iterator, then iterates over the items of each result iterator consecutively. Equivalent to `citer_flatten(citer_map(it, fn))`. |
| flatten    | I | Flattens an iterator of iterators into a single iterator.                                                           |
| inspect    | I | Calls a callback function on each item of an iterator, without modifying the returned items.                        |
| map        | I | Maps each item of an iterator using a callback function.                                                            |
| once       | Y | Iterator which returns a given item once. Equivalent to `citer_take(citer_repeat(item), 1)`.                        |
| over_array | Y | Iterates over the items in an array. Returns a pointer to each item in the array as the item.                       |
| repeat     | Y | Iterator which repeatedly returns the same item.                                                                    |
| reverse    | Y | Iterator which reverses a double-ended iterator.                                                                    |
| skip       | I | Skips the first N items of another iterator.                                                                        |
| skip_while | N | Skips the items of another iterator until a given predicate function returns false.                                 |
| take       | N | Iterates over the first N items of another iterator.                                                                |
| take_while | N | Iterates over items of another iterator until a given predicate function returns false.                             |
| zip        | N | Zips two iterators together, returning pairs of items, one from each input iterator.                                |

\*
Y: Yes,
N: No,
I: Inherited (i.e. double-ended if all input iterators are double-ended)

### Functions

| Function   | Description                                                                           |
| ---        | ---                                                                                   |
| all        | Returns true if all items of an iterator satisfy a given predicate function.          |
| any        | Returns true if any items of an iterator satisfy a given predicate function.          |
| collect_into_array       | Collects the items of an iterator into an array.                                      |
| collect_into_linked_list | Collects the items of an iterator into a linked list.                                 |
| count      | Counts the number of items in an iterator.                                            |
| find       | Returns the first item of an iterator satisfying a given predicate function.          |
| fold       | Accumulate all items of an iterator into a single value using a given function.       |
| free       | Frees (de-allocates) an iterator and its associated data.                             |
| free_data  | Frees the data associated with an iterator, but not the iterator structure itself.    |
| is_double_ended | Checks if an iterator is double-ended.                                           |
| max        | Returns the maximum item of an iterator, comparing using a given comparison function. |
| min        | Returns the minimum item of an iterator, comparing using a given comparison function. |
| next       | Returns the next item of the iterator.                                                |
| next_back  | Returns the next item from the back of a double-ended iterator.                       |
| nth        | Returns the Nth item of an iterator.                                                  |
| nth_back   | Returns the Nth item from the end of a double-ended iterator.                         |
