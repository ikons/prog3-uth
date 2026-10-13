#!/bin/bash
# οι συμβολικοί σύνδεσμοι ενός καταλόγου
OUTFILE=symlinks.list
directory=${1-$(pwd)}
for file in "$(find "$directory" -type l)"
do
  echo "$file"
done | sort >> "$HOME/$OUTFILE"
exit 0
