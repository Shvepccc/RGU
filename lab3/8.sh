#!/bin/bash

function print_help {
    echo "This function outputs all subdirectories in the specified directory by sorting them by the size occupied by their files."
    echo "Flags:
    -d specifying the start directory of the search (default: current directory)"
    echo ""
}

error_file=""
search_directory=""

while getopts "d:e:h" opt; do
    case $opt in
        d)
            search_directory="$OPTARG";
            ;;
        e)
            error_file="$OPTARG";
            echo "error_file: $error_file"
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

if [ -n "$error_file" ]; then
    exec 2>>"$error_file"
fi

if [[ -d "$search_directory" ]]; then
    du -h -d 1 "$search_directory" | sort -h
else 
    echo "Error: $search_directory not found"
fi