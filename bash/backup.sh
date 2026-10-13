#!/bin/bash
BCKUP=$HOME/backup-$(date +%F).tar.gz
tar -czf "$BCKUP" "$HOME/work"
echo "Saved $BCKUP"
