#!/bin/bash

usage() {
    echo "Writes a string into a file"
    echo "Usage:"
    echo "$0 <writefile> <writestr>"
    echo 
    echo "writefile    File to write to"
    echo "writestr     String to write into the file"
}

writefile=$1
writestr=$2

if [ $# != 2 ]; then
    echo "Expected 2 parameters. $# Given"
    usage
    exit 1
fi

mkdir -p "$(dirname "${writefile}")" && echo ${writestr} > ${writefile}

if [ $? != 0 ]; then
    echo "File could not be created"
    exit 1
fi