#!/bin/bash

val=1
if [ $# -eq 2 ]; then
    if [ $1 == "-e" ]; then
        2>$2
    fi
fi
if [[ -n "$1" && $1 == "-h" ]]; then
    val=0
    echo "3.sh counts the number of files located in the specified path (including subdirectories)."
    echo "Using the -d flag, you can specify the directory, otherwise the current one is selected by default."
fi

if [ $val -eq 1 ]; then
    DIR=${1:-.}
    file_count=$(find "$DIR" -type f | wc -l)
    echo "Number of files: $file_count"
fi