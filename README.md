# CIter: Lazily-evaluated iterators for C

Inspired by Rust, this library attempts to implement lazily-evaluated iterators
in C.

## Usage

To use this library, include the header file `citer.h` in your source code and link against either `libciter.a` or `libciter.so`.
The former is a static library, while the latter is dynamically linked.

To link against the dynamic library once it has been installed, add `-lciter` to your compiler/linker flags (usually `LDLIBS`).
To link against the static library, add `-l:libciter.a`, and specify the directory the library is in using `-L/path/to/libraries`.

## Getting CIter

Currently, CIter must be built from sources.
Don't worry though; it's easy!

### Building

Simply run `make` in the root directory of the repository.

This will build three notable files: `citer.h`, `libciter.a`, and `libciter.so.0.0.0`.

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
 - install `libciter.so.0.0.0` as `$(PREFIX)/lib/libciter.so.0.0.0`;
 - create symbolic links `libciter.so` and `libciter.so.0` to `libciter.so.0.0.0` in `$(PREFIX)/lib`.

### Uninstalling

To uninstall CIter, run `make uninstall`.
Just like when installing, set the `PREFIX` accordingly.

## Implemented so far

All iterators and functions below are prefixed with the name `citer_` to avoid clobbering the global namespace.

### Iterators

| Iterator   | Description                                                                                                         |
| ---        | ---                                                                                                                 |
| chain      | Chains two iterators. Iterates over all items of the first, then all items of the second.                           |
| chunked    | Iterates over N-item chunks of an iterator at a time.                                                               |
| enumerate  | Enumerates the items of an iterator. Each new item is a `citer_enumerate_item_t` containing the index and the item. |
| filter     | Filters items of an iterator using a predicate function.                                                            |
| flat_map   | Maps each item of an iterator to an iterator, then iterates over the items of each result iterator consecutively.   |
| map        | Maps each item of an iterator using a callback function.                                                            |
| over_array | Iterates over the items in an array. Returns a pointer to each item in the array as the item.                       |
| repeat     | Iterator which repeatedly returns the same item.                                                                    |
| skip       | Skips the first N items of another iterator.                                                                        |
| take       | Iterates over the first N items of another iterator.                                                                |

### Functions

| Function   | Description                                                                           |
| ---        | ---                                                                                   |
| all        | Returns true if all items of an iterator satisfy a given predicate function.          |
| any        | Returns true if any items of an iterator satisfy a given predicate function.          |
| free       | Frees (de-allocates) an iterator and its associated data.                             |
| free_data  | Frees the data associated with an iterator, but not the iterator structure itself.    |
| max        | Returns the maximum item of an iterator, comparing using a given comparison function. |
| min        | Returns the minimum item of an iterator, comparing using a given comparison function. |
| next       | Returns the next item of the iterator.                                                |
| nth        | Returns the Nth item of an iterator.                                                  |
