#!/bin/bash

for f in ./calgary/*
do
  echo "Processing $f"
  ./verify.sh $f
done
