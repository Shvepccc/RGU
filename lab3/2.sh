#!/bin/bash

val=1
if [ $# -eq 2 ]; then
    if [ $1 == "-e" ]; then
        2>$2
    fi
fi
if [[ -n "$1" && $1 == "-h" ]]; then
    val=0
    echo "Run me to count the number of lines in the files passed as arguments."
    echo "Parametrs: names of files."
fi

if [ $val -eq 1 ]; then
    str_count=0
    for param in "$@"; do 
        #echo "\$@ Parameter #$str_count = $param"
        var=$(grep -c $ $param)
	    str_count=$(( $str_count + $var)) 
    done
    echo "Number of lines in files: $str_count"
fi