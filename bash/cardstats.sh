#!/bin/bash
N=100000
hits=(0 0 0 0)          # μετρητές
if [ $# -gt 0 ]; then   # όρισμα;
  N=$1
else                    # αλλιώς ρώτα
  read -t 5 -p "Number of trials: " N
  N=${N:-100000}        # κενό: προεπιλογή
fi
i=$N
echo "Generating $N random numbers..."
SECONDS=0               # χρονόμετρο
while [ $i -gt 0 ]; do
  case $((RANDOM%4)) in   # 0 ως 3
    0) let "hits[0]+=1";;
    1) let "hits[1]=${hits[1]}+1";;
    2) let hits[2]=$((${hits[2]}+1));;
    3) (( hits[3]++ ));;
  esac
  let "i-=1"              # αντίστροφα
done
echo "Probabilities of drawing a specific color:"
echo "Clubs:    $(echo "${hits[0]}*100/$N" | bc -l)"
echo "Diamonds: $(echo "${hits[1]}*100/$N" | bc -l)"
echo "Hearts:   $(echo "${hits[2]}*100/$N" | bc -l)"
echo "Spades:   $(echo "${hits[3]}*100/$N" | bc -l)"
echo "Execution time: $SECONDS"
