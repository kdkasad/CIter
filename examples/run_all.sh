#!/bin/bash

#
# CIter - C library for lazily-evaluated iterators.
# Copyright (C) 2024  Kian Kasad <kian@kasad.com>
#
# This file is part of CIter.
#
# CIter is free software: you can redistribute it and/or modify it under the terms
# of the GNU Lesser General Public License as published by the Free Software
# Foundation, version 3 of the License.
#
# CIter is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along
# with CIter. If not, see <https://www.gnu.org/licenses/>.
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
run ./enumerate a b c d
run ./map 1 2 3 4
run ./chunked 3 {1..20}
run ./filter 2 1 2 3 4 5 6 7 8 9 10
run ./minmax {1..50}
run ./collect item 5
run ./count_using_fold {a..z}
run ./sum {1..5}
run ./inspect {1..5}
run ./skip_take_while {1..100}
run ./reverse {a..f}
