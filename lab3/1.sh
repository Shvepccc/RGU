#!/bin/bash

val=1
if [ $# -eq 2 ]; then
    if [ $1 == "-e" ]; then
        2>$2
    fi
fi
if [[ -n "$1" && $1 == "-h" ]]; then
    val=0
    echo "Launch me to view a table of degrees (from 0 to 6) of natural numbers in the range from 1 to 9 in the form of a table"
fi

if [ $val -eq 1 ]; then
    echo -ne "num\t0\t1\t2\t3\t4\t5\t6\n"
    echo
    for n in {1..9}; do
        echo -n "$n"
        for power in {0..6}; do
            res=$((n ** power))
            echo -ne "\t$res"
        done
        echo
    done
fi