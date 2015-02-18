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

# remove file with previous results
if [ -d "$RES_FILE" ]; then
    rm "$RES_FILE"
fi

# iterate over test files
FILES="$DIR""*.wav"

for FILE in $FILES
do
    # launch application on test file
    #ANGLE=` $APP_PATH./sound_direction listen-file -c $DIST -f $FILE `
    ANGLE=` $APP_PATH./sound_direction benchmark -f "$FILE" -d 0 `

    # extract name of file 
    FILENAME=$(basename "$FILE" | cut -d'.' -f1)
    DESTDIR="$DEST/$FILENAME"
    mkdir "$DESTDIR"
    
    STR="$FILE : $ANGLE"
    # output    
    echo $STR
    echo $STR >> $RES_FILE

    # move files with graphs data
    TEST_PATH="./"
    TEST_EXT="*.test"
    GRAPH_EXT="*.png"    
    if [ `find "$TEST_PATH" -maxdepth 1 -type f -name "$TEST_EXT" | wc -l` != 0 ]; then 
        # for each .test file plot graph
        for GRAPH in "$TEST_PATH"$TEST_EXT
        do
            python ./plot.py $GRAPH            
            rm $GRAPH
        done

        mv "$TEST_PATH"$GRAPH_EXT "$DESTDIR"
    fi 
        
done
