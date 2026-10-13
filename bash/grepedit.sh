#!/bin/bash
if [ $# -le 1 ]
then
  echo "Usage: $0 pattern file ..."; exit 1
else
  pattern=$1          # κράτα το αρχικό $1
  shift               # όλα μία θέση αριστερά
  while [ $# -gt 0 ]  # το νέο $1: το 1ο αρχείο
  do
    grep "$pattern" "$1" > /dev/null
    if [ $? -eq 0 ]; then   # το βρήκε;
      vi "$1"
    fi
    shift
  done
fi
