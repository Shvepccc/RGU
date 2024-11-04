#!/bin/bash

function print_help {
    echo "The function creates files based on data from the configuration file."
    echo "Only the file name must be present in the line. If any parameter is omitted, the default value is used:"
    echo "   *for the destination directory, the user's home directory,"
    echo "   *for access rights — 0644,"
    echo "   *for the user and the group — the current values."
    echo ""
    echo "Flags:
    -h — help output
    -e — redirecting the error stream to the specified file
    -m — change the default access rights to the one specified after the key
    -M — change all access rights to the one specified after the key
    -u — change the default user to the one specified after the key
    -U — change all users to the one specified after the key
    -g — change the default group to the one specified after the key
    -G — changes all groups to the one specified after the key"
    echo ""     
    #
}

error_file=""
config_file=""

change_user=0
change_access_rights=0
change_group=0

default_access_rights="0644"
default_user=$USER
groups=$(id -nG)
default_group="${groups%% *}"

while getopts "e:hm:u:g:M:G:U:" opt; do
    case $opt in
        e)
            error_file="$OPTARG";
            echo "error_file: $error_file"
            ;;
        h)
            print_help;
            exit 0;
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
        M)
            default_access_rights="$OPTARG";
            change_access_rights=1;
            ;;
        U)
            default_user="$OPTARG";
            change_user=1;
            ;;
        G)
            default_group="$OPTARG";
            change_group=1;
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

if [[ -n "$1" ]]; then
    config_file=$1
    while IFS= read -r line
    do
        IFS=" : "
        read CUSTOM_DIR CUSTOM_RIGHTS CUSTOM_USERNAME CUSTOM_GROUP <<< "$line"

        if [[ $CUSTOM_DIR == "" ]]; then
            echo "The absolute path of the file is required!"
            exit 1
        fi
        if [[ $CUSTOM_RIGHTS == "" || $change_access_rights -eq 1 ]]; then
            CUSTOM_RIGHTS=$default_access_rights
        fi
        if [[ $CUSTOM_USERNAME == "" || $change_user -eq 1 ]]; then
            CUSTOM_USERNAME=$default_user
        fi
        if [[ $CUSTOM_GROUP == "" || $change_group -eq 1 ]]; then
            CUSTOM_GROUP=$default_group
        fi

        touch "$CUSTOM_DIR"
        chmod "$CUSTOM_RIGHTS" "$CUSTOM_DIR"
        chown "$CUSTOM_USERNAME":"$CUSTOM_GROUP" "$CUSTOM_DIR"

        echo "File craeted by path $CUSTOM_DIR with rights $CUSTOM_RIGHTS, owner $CUSTOM_USERNAME and group $CUSTOM_GROUP"

        unset IFS
    done < $config_file
else
    echo "You must specify all the parameters!"
fi