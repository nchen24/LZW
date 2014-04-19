#!/bin/sh

inFile=$1
option=$2
if [[ "$option" == a* ]]; then
  ./a.out -c $inFile > out; ./a.out -x out > fin; diff fin $inFile;
else
  ./b.out -c $inFile > out; ./b.out -x out > fin; diff fin $inFile;
fi

set -- $(du -sk "out")
outSize=$1
set -- $(du -sk $inFile)
inSize=$1

echo "File: $inFile"
echo "\tOriginal: $inSize\tCompressed: $outSize"
rm fin out
