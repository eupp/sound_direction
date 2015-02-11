#!/bin/bash

# microphone distance
DIST="$1"
# path to application
APP_PATH="$2"

# directory with test files
DIR="wav/"
# output directory
DEST="test_results"
# file with summary results
RES_FILE="$DEST"/result.txt

# create directory for test results (and remove old one)
if [ -d "$DEST" ]; then
    rm -rf "$DEST"
fi
mkdir "$DEST"

# iterate over test files
FILES="$DIR""*.wav"

for FILE in $FILES
do
    ANGLE=` $APP_PATH./sound_direction listen-file -c $DIST -f $FILE `
    
    # extract name of file 
    FILENAME=$(basename "$FILE" | cut -d'.' -f1)
    DESTDIR="$DEST/$FILENAME"
    mkdir "$DESTDIR"
    
    STR="$FILE : $ANGLE"
    # output    
    echo $STR
    echo $STR >> $RES_FILE

    # move files with graphs data
    TEST_EXT="*.test"    
    if [ `find "$APP_PATH" -maxdepth 1 -type f -name "$TEST_EXT" | wc -l` != 0 ]; then 
        mv "$APP_PATH"$TEST_EXT "$DESTDIR"
    fi 
        
done
