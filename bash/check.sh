#!/bin/bash
for f in *.sh; do
  if [ -x "$f" ]; then
    echo "$f is executable"
  fi
done
