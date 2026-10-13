#!/bin/bash
for filename in *                  # όλα τα αρχεία
do
  fname=$(basename "$filename")    # χωρίς διαδρομή
  n=$(echo "$fname" | tr A-Z a-z)  # σε πεζά
  if [ "$fname" != "$n" ]          # μόνο όσα αλλάζουν
  then
    mv -i -- "$fname" "$n"
  fi
done
exit 0
