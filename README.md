# Lazily-evaluated iterators for C

Inspired by Rust, this library attempts to implement lazily-evaluated iterators
in C.

## Implemented so far

| Module | Description |
| ---    | ---         |
| chain  | Chains two iterators. Returns all items of the first, then all items of the second. |
| over_array | Iterates over the items in an array. |
