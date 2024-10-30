#!/bin/bash

function print_help {
    echo "help text"
}

error_file=""
filename=""

while getopts "e:h" opt; do
    case $opt in
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

if [ -n "$error_file" ]; then
    exec 2>>"$error_file"
fi

target=$1
shift

for param in "$@"; do 
    if [ -d "$param" ]; then
        echo "Files in directory: \"$param\""
        #find $param -type f
        #find . -type f ! -path '*/.*'
        for file in $(find $param -type f); do 
            #echo $file
            if [ -f "$file" ]; then 
                word_count=$(wc -w < "$file")
                #echo $word_count
                if [ $word_count -gt $target ]; then
                    realpath -s --relative-to="$param" "$file"
                fi 
            fi 
        done 
    else
        echo "$param is not a directory"
    fi
done
