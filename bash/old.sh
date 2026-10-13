#!/bin/bash
if [ $# -eq 0 ]          # υπάρχουν ορίσματα;
then
  echo "Usage: $0 file ..."
  exit 1
fi
if [ ! -d "$HOME/old" ]
then
  mkdir "$HOME/old"
fi
echo The following files will be saved in the old directory:
echo "$@"
for file in "$@"         # για κάθε όρισμα
do
  mv "$file" "$HOME/old/"
  chmod 400 "$HOME/old/$file"
done
ls -l "$HOME/old"
