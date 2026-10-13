#!/bin/bash
if [ -z "$1" ]; then
  echo "Usage: $0 arg1 arg2 ..."; exit 1
fi
index=1
echo "Listing args with \"\$*\":"
for arg in "$*"; do
  echo "Arg $index = $arg"
  let "index+=1"
done
echo "Entire arg list seen as single word."
index=1
echo "Listing args with \"\$@\":"
for arg in "$@"; do
  echo "Arg $index = $arg"
  let "index+=1"
done
echo "Arg list seen as separate words."
