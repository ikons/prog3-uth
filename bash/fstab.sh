#!/bin/bash
if [ -f /etc/fstab ]; then
  cp /etc/fstab .
  echo "Done."
else
  echo "This file does not exist."
  exit 1
fi
