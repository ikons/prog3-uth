#!/bin/bash
echo -n "Enter a number: "; read x
let sum=0; let i=1
while [ $i -le $x ]; do
  let "sum = $sum + $i"
  i=$(( $i + 1 ))
done
echo "the sum of the first $x numbers is: $sum"
