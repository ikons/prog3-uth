#!/bin/bash
if [ $# -eq 1 ]; then
  if [ ! -d "$HOME/trash" ]; then
    mkdir "$HOME/trash"
  fi
  mv "$1" "$HOME/trash"
else
  echo "Use: $0 filename"
  exit 1
fi
