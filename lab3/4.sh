#!/bin/bash

function print_help {
    echo "In order to determine which directories contain a file with the specified name, specify the file name and run the program."
}

start_dir="."
error_file=""
filename=""

while getopts "d:e:h" opt; do
    case $opt in
        d)
            start_dir="$OPTARG"
            ;;
        e)
            error_file="$OPTARG"
            ;;
        h)
            print_help;
            exit 0;
            ;;
        *)
            print_help;
            exit 1;
            ;;
    esac
done

shift $((OPTIND - 1))

if [ -z "$1" ]; then
    echo "Error: You have to pass on the name of the file" >&2
    print_help
    exit 1
fi

filename="$1"

if [ -n "$error_file" ]; then
    exec 2>>"$error_file"
fi

if [ ! -d "$start_dir" ]; then
    echo "Error: Directory '$start_dir' not found" >&2
    exit 1
fi

find "$start_dir" -type f -name "$filename" -exec dirname {} \; | sort -u
