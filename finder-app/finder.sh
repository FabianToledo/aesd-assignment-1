#!/bin/sh

usage() {
    echo "Searches into all the files in the directory for a string"
    echo "Usage:"
    echo "$0 <filesdir> <searchstr>"
    echo 
    echo "filesdir     Directory to find the search string"
    echo "searchstr    The string to look for"
}

filesdir=$1
searchstr=$2

if [ $# != 2 ]; then
    echo "Expected 2 parameters. $# Given"
    usage
    exit 1
fi

if [ ! -d $1 ]; then
    echo "$1 is not a directory"
    usage
    exit 1
fi

TMPFILE=$(mktemp --tmpdir finder.XXXXXX)

xcount=0
ycount=0
for file in $(find ${filesdir} -type f -print); do
    xcount=$((xcount+1))
    grep "${searchstr}" ${file} >> ${TMPFILE}
done
ycount=$(wc -l < ${TMPFILE})

rm ${TMPFILE}

echo "The number of files are $xcount and the number of matching lines are $ycount"
