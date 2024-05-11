#!/bin/bash 
for i in *
do
    if [ -d "$i" ] 
    then
        FILES=`ls -l "$i" | wc -l`
        FILES2=`expr $FILES - 1`
        if [ $FILES2 -eq -1 ] 
        then
            FILES2=0 
        fi
        echo "$i: $FILES2"
    fi 
done