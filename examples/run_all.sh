#!/bin/sh

#
# Run all examples
#

set -e

cd $(dirname "$0")

run() {
    printf '%s\n' "$*"
    "$@"
    echo
}

run ./repeat_take item 7
run ./over_array a bb ccc dddd
run ./chain a 3 b 4
run ./map 1 2 3 4
