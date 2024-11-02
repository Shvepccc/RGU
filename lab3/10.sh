#!/bin/bash

function print_help {
    echo "A catalog and a settings file are submitted for input, in which information received from"
    echo "the user in interactive mode is added. The default settings file is called .install-config."
    echo "For each file from the specified directory, the name of the destination directory, access rights,"
    echo "user name and its group are asked. If the user has not entered anything, the default value is saved:"
    echo "   *for the destination directory, the user's home directory,"
    echo "   *for access rights — 0644,"
    echo "   *for the user and the group — the current values."
    echo ""
    echo "Flags:
    -h help output
    -e redirecting the error stream to the specified file
    -o redefining the settings file. 
    -a process automation. All values are assumed to be the default values
    -p changing the default path to the one specified after the key
    -m change the default access rights to the one specified after the key 
    -u change the default user to the one specified after the key 
    -g change the default group to the one specified after the key"
    echo ""     
    #./10.sh -a . .install-config
}

error_file=""
config_file=""
search_directoty=""
automaticly=0

default_dir=$HOME
default_access_rights="0644"
default_user=$USER
default_group=$GROUP

while getopts "e:ho:ap:m:u:g:" opt; do
    case $opt in
        e)
            error_file="$OPTARG";
            echo "error_file: $error_file"
            ;;
        h)
            print_help;
            exit 0;
            ;;
        o)
            config_file="$OPTARG";
            ;;
        a)
            automaticly=1;
            ;;
        p)
            default_dir="$OPTARG";
            ;;
        m)
            default_access_rights="$OPTARG";
            ;;
        u)
            default_user="$OPTARG";
            ;;
        g)
            default_group="$OPTARG";
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

if [[ -n "$1" && -n "$2" ]]; then
    search_directoty=$1
    echo $search_directoty
    config_file=$2
    > $config_file
    for file in $(find $search_directoty -type f); do
        if [ $automaticly -eq 0 ]; then
            echo -e "\n"
            echo "----------------------------"
            echo "File: $file"
            read -p "Enter name of the destination directory: " CUSTOM_DIR
            read -p "Enter access rights: " CUSTOM_RIGHTS
            read -p "Enter username: " CUSTOM_USERNAME
            read -p "Enter group: " CUSTOM_GROUP
        fi

        if [[ $CUSTOM_DIR == "" || $automaticly -eq 1 ]]; then
            CUSTOM_DIR=$default_dir
        fi
        if [[ $CUSTOM_RIGHTS == "" || $automaticly -eq 1 ]]; then
            CUSTOM_RIGHTS=$default_access_rights
        fi
        if [[ $CUSTOM_USERNAME == "" || $automaticly -eq 1 ]]; then
            CUSTOM_USERNAME=$default_user
        fi
        if [[ $CUSTOM_GROUP == "" || $automaticly -eq 1 ]]; then
            CUSTOM_GROUP=$(ls -l $file | awk '{print $4}')
        fi
        file="${file#.}"
        CUSTOM_DIR="$CUSTOM_DIR$file"
        echo "$CUSTOM_DIR : $CUSTOM_RIGHTS : $CUSTOM_USERNAME : $CUSTOM_GROUP" | tee -a $config_file
    done
else
    echo "You must specify all the parameters!"
fi