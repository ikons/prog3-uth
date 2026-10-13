#!/bin/bash
echo -n "Enter your login name: "
read name
if [ "$name" = "$USER" ]; then
  echo "Hello, $name. How are you today?"
else
  echo "You are not $USER, so who are you?"
fi
