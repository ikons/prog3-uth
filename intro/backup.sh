#!/bin/sh
for f in *.txt; do
    cp "$f" "$f.bak"
done
echo "Έτοιμα τα αντίγραφα ασφαλείας"
