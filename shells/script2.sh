#!/bin/bash
echo the name of this script is $0
echo the first argument is $1
echo a list of all the arguments is $*
echo this script places the date into
echo a temporary file called $1.$$
date > $1.$$     # ανακατεύθυνση του date
ls $1.$$         # λίστα του αρχείου
rm $1.$$         # διαγραφή του αρχείου
