#!/bin/bash
cat << ENDOFTEXT
Dear $1,
Please see me regarding some exciting news!
- $USER
ENDOFTEXT
echo letter written for $1
