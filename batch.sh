#!/bin/bash

for f in ~/projects/testObjects/calgary/*
do
  echo "Processing $f"
  ./verify.sh $f
done
