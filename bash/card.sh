#!/bin/bash
# τα χρώματα και οι αξίες της τράπουλας
Suites="Clubs Diamonds Hearts Spades"
Denominations="2 3 4 5 6 7 8 9 10 Jack Queen King Ace"
# ανάγνωση σε μεταβλητές πίνακα
suite=($Suites)
denomination=($Denominations)
# καταμέτρηση του αριθμού των στοιχείων
num_suites=${#suite[*]}
num_denominations=${#denomination[*]}
echo -n "${denomination[$((RANDOM%num_denominations))]} of "
echo ${suite[$((RANDOM%num_suites))]}
exit 0
