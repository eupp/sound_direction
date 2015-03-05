#!/bin/bash

# microphone distance
DIST="$1"
# path to application
APP="$2"

# directory with test files
DIR="wav/"
# output directory
DEST="test_results"
# file with summary results
RES_FILE="$DEST"/result.txt

APP_PATH=` dirname $APP`
export LD_LIBRARY_PATH="$APP_PATH:$LD_LIBRARY_PATH"

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
    ANGLE=` "$APP" listen-file -c $DIST -f $FILE `
    #ANGLE=` "$APP" benchmark -f "$FILE" -d 0 `

    # extract name of file 
    FILENAME=$(basename "$FILE" | cut -d'.' -f1)
    DESTDIR="$DEST/$FILENAME"
    mkdir "$DESTDIR"
    
    STR="$FILE : $ANGLE"
    # output    
    echo $STR
    echo $STR >> $RES_FILE

    TEST_PATH="./"
    TEST_EXT=".test"

    #move file with history statistics
    TEST_WINDOW="$TEST_PATH""testWindow""$TEST_EXT"
    if [ -f $TEST_WINDOW ]; then
        mv "$TEST_WINDOW" "$DESTDIR" 
    fi

    # move files with graphs data
    TEST_EXT=*"$TEST_EXT"    
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
