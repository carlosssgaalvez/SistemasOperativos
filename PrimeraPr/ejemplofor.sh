#!/bin/bash

for entry in ./*
do
  if [ -f "$entry" ]; then
    echo "$entry is a file"
  elif [ -d "$entry" ]; then
    echo "$entry is a directory"
  fi
done