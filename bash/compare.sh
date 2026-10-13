#!/bin/bash
ARGS=2                        # δύο ορίσματα
if [ $# -ne "$ARGS" ]; then
  echo "Usage: $(basename "$0") file1 file2"; exit 1
fi
if [[ ! -r "$1" || ! -r "$2" ]]; then
  echo "Both files must exist and be readable."; exit 2
fi
cmp "$1" "$2" &> /dev/null    # χωρίς έξοδο
if [ $? -eq 0 ]               # ο κωδικός του cmp
then
  echo "File \"$1\" is identical to file \"$2\"."
else
  echo "File \"$1\" differs from file \"$2\"."
fi
exit 0
