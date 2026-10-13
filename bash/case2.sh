#!/bin/bash
echo -n "Enter a number 1 < x < 10: "
read x
case $x in
  [1-9]) echo "Value of x is $x.";;
  0 | 10) echo "wrong number.";;
  *) echo "Unrecognized value.";;
esac
