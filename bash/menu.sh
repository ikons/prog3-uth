#!/bin/bash
clear; loop=y
while [ "$loop" = y ]; do
  echo "Menu"; echo "===="
  echo "D: print the date"
  echo "W: print the users who are logged on"
  echo "P: print the working directory"
  echo "Q: quit."
  echo
  read -s choice    # χωρίς ηχώ στο τερματικό
  case $choice in
    D | d) date ;;
    W | w) who ;;
    P | p) pwd ;;
    Q | q) loop=n ;;
    *) echo "Illegal choice." ;;
  esac
  echo
done
