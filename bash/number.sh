#!/bin/bash
echo -n "Enter a number 1 < x < 10: "
read num
if [ "$num" -lt 10 ]; then
  if [ "$num" -gt 1 ]; then
    echo "$num*$num=$(($num*$num))"
  else
    echo "Wrong insertion !"
  fi
else
  echo "Wrong insertion !"
fi
