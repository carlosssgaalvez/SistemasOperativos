#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Ingresa un directorio para analizar"
    exit 1
fi

if [ ! -d "$1" ]
then
    echo "El parametro es erroneo"
    exit 1
fi

for i in "$1"/*
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