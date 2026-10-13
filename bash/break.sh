#!/bin/bash
LIMIT=19
echo "1 to 20, but stop after 2:"
a=0
while [ $a -le "$LIMIT" ]; do
  a=$(($a+1))
  if [ "$a" -gt 2 ]; then
    break
  fi
  echo -n "$a "
done
echo
