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

argv0="$0"

cd $(dirname "$0")

pass_count=0
run_count=0

usage() {
    cat << EOF
Usage:
    $argv0 [-p|--print-fails] [-n|--no-save] [--] [<test>...]

Options:
    -p, --print-fails
        Print output of failed runs

    -n, --no-save
        Don't save test output in <test>.out files
EOF
    exit 2
}

run() {
    run_count=$(( $run_count + 1 ))
    printf 'Running %-24s\t' "$1..."
    ./$1 >$1.out 2>&1
    if [ $? = 0 ]
    then
        pass_count=$(( $pass_count + 1 ))
        printf '\x1b[1;32mPassed\x1b[m\n'
    else
        printf '\x1b[1;31mFailed\x1b[m\n'

        # Print output if -p option is set
        if [ "${opt_print_fails:+x}" = x ]
        then
            cat $1.out
            echo
        fi
    fi
    if [ "${opt_no_save:+x}" = x ]
    then
        rm -f $1.out
    fi
}

opt_print_fails=$PRINT_ON_FAIL
opt_no_save=$NO_SAVE_OUTPUT
while :
do
    case "$1" in
        -p|--print-fails) opt_print_fails=1 ;;
        -n|--no-save) opt_no_save=1 ;;
        -h|-\?|--help) usage ;;
        --) break ;;
        -? | --* ) usage ;;
        *) break ;;
    esac
    shift
done

for test in "$@"
do
    run $test
done

printf '\nPassed %d out of %d tests.\n' $pass_count $run_count

if [ $pass_count = $run_count ]
then
    exit 0
else
    exit $(( $run_count - $pass_count ))
fi
