#!/bin/bash

function print_help {
    echo "This function accepts the names of the processes that need to be terminated."
     echo ""
    echo "Flags:
    -h help output
    -e redirecting the error stream to the specified file
    -s changing the signal being sent"
    echo ""
}

error_file=""
signal=15

while getopts "e:hs:" opt; do
    case $opt in
        e)
            error_file="$OPTARG"
            ;;
        h)
            print_help;
            exit 0;
            ;;
        s)
            signal="$OPTARG";
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
 
pids=$(pgrep "$1")
if [ -z "$pids" ]; then
  echo "Process with name '$1' not found."
  exit 0
fi

for pid in $pids; do
  kill -$signal $pid
  echo "Send signal $signal to process with process $pid"
done