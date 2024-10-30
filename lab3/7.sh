#!/bin/bash

function print_help {
    echo "This function creates links to each of the files where the specified string is found."
    echo "The required string is specified after the introduction of all flags!"
    echo "Flags:
    -d specifying the start directory of the search (default: current directory) 
    -o specifying the directory for saving links (default: current directory)
    -s creation of soft links (if not specified, then hard links are created)
    -e redirecting the error stream to the specified file
    -r deleting link files (to delete link files from the specified directory, add the -d forward -r flag)"
    echo ""

    #./7.sh -d ~/RGU/ -r
    #/7.sh -d ~/RGU/ -o ~/RGU/ Hello
}

function delete_files {
    if [ $search_directory == "." ]; then
        rm -f *.link
    else
        rm "$search_directory"*."link"
    fi
    echo "All link files was deleted successfully!"
}

error_file=""
filename=""
link_directory="."
link_type=0
search_directory="."

while getopts "rd:e:hso:" opt; do
    case $opt in
        r)
            delete_files;
            exit 0;
            ;;
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
        s)
            link_type=1;
            ;;
        o)
            link_directory="$OPTARG";
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

string=$1
shift

prom="/"
for param in $(grep -rl $string $search_directory); do 

    file_name="${param##*/}"
    new_filename="${file_name%.*}.link"

    if [ $link_directory == "." ]; then
        new_path="$new_filename"
    else
        new_path="$link_directory$new_filename"
    fi

    if [ $link_type -ne 0 ]; then
        echo "new soft link: $new_path"
        ln -s $param $new_path
    else
        echo "new hard link: $new_path"
        ln $param $new_path
    fi
done
