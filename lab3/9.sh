#!/bin/bash

function print_help {
    echo "This function deletes all files according to the list of files located in the specified file."
    echo "Flags:
    -d specifying the start directory of the search (default: current directory)"
    echo ""
}

error_file=""

while getopts "e:h" opt; do
    case $opt in
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

if [[ -n "$1" ]]; then
    while read -r line; do
        line="${line/#\~/$HOME}"
        echo "Deleting: $line"
        rm $line
    done < $1
else
    echo "You must specify the file"
fi