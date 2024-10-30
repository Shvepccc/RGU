#!/bin/bash

function print_help {
    echo "For the list of transferred files, the function creates hard links with the same name, but with the added extension ".1""
    echo "If such links already exist, the extension is increased by 1: ".2"" 
    echo "If there are all files with the extension up to and including ".9", then new links are not created" 
    echo "The -r key removes all hard links to files with permissions from ".1" to ".9""
}

function delete_files {
    for number in {1..9}; do
        rm -f *.$number
    done
    echo "All link files was deleted successfully!"
}

error_file=""
filename=""

while getopts "re:h" opt; do
    case $opt in
        r)
            delete_files;
            exit 0;
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

if [ -n "$error_file" ]; then
    exec 2>>"$error_file"
fi

for param in "$@"; do 
    if [ -f "$param" ]; then
        #echo "file: $param"
        for number in {1..9}; do
            new_filename="${param%.*}.$number"
            if [ -f $new_filename ]; then
                continue
            else
                #echo "new link: $new_filename"
                ln $param $new_filename
                break
            fi
        done
    else
        echo "$param is not a file"
    fi
done
