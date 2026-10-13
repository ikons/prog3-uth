#!/bin/bash
echo -n "Enter a number 1 < x < 10: "
read x
case $x in
  1) echo "Value of x is 1.";;
  2) echo "Value of x is 2.";;
  3) echo "Value of x is 3.";;
  4) echo "Value of x is 4.";;
  5) echo "Value of x is 5.";;
  6) echo "Value of x is 6.";;
  7) echo "Value of x is 7.";;
  8) echo "Value of x is 8.";;
  9) echo "Value of x is 9.";;
  0 | 10) echo "wrong number.";;
  *) echo "Unrecognized value.";;
esac
