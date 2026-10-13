#!/usr/bin/env bash
# Μεταγλωττίζει όλα τα παραδείγματα όπως γράφουν τα README και ελέγχει τη σύνταξη
# των σκριπτ. Δουλεύει σε προσωρινό αντίγραφο, ώστε ο κατάλογος να μένει καθαρός.
# Στο image του Dockerfile:
#   docker run --rm -v "$PWD":/home/student/examples prog3 ./check.sh
set -euo pipefail

src=$(cd "$(dirname "$0")" && pwd)
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
cp -r "$src"/. "$work"
cd "$work"

failed=0
ok()   { printf '  ok    %s\n' "$*"; }
fail() { printf '  FAIL  %s\n' "$*"; failed=1; }
# Η εντολή πρέπει να πετύχει.
run()  { if "$@" > /dev/null 2>&1; then ok "$*"; else fail "$*"; "$@" 2>&1 | head -5 | sed 's/^/        /'; fi; }
# Η εντολή πρέπει να αποτύχει: τα σκόπιμα λάθη των διαλέξεων.
must_fail() { if "$@" > /dev/null 2>&1; then fail "$*: έπρεπε να αποτύχει"; else ok "$*: αποτυγχάνει σκόπιμα"; fi; }
# Η εντολή πρέπει να τυπώσει ακριβώς αυτό.
prints() { local want=$1; shift; if [ "$("$@" 2>&1)" = "$want" ]; then ok "$*"; else fail "$*: περίμενα «$want»"; fi; }

CFLAGS=(-std=c17 -Wall -Wextra -Werror -g)

echo "== Σκριπτ (bash -n, ksh -n)"
for f in intro/*.sh shells/*.sh bash/*.sh; do run bash -n "$f"; done
run ksh -n shells/script.ksh
prints 'Hello World' bash/hello.sh
prints 15 bash/sum.sh

echo "== Ένα πρόγραμμα ανά αρχείο C"
for d in intro syscalls processes signals pipes; do
    for f in "$d"/*.c; do run gcc "${CFLAGS[@]}" "$f" -o "${f%.c}"; done
done
for d in sockets threads; do
    for f in "$d"/*.c; do run gcc "${CFLAGS[@]}" -pthread "$f" -o "${f%.c}"; done
done
run python3 -m py_compile ./*/*.py

echo "== Εργαλεία (12.1)"
t=tools/single
run gcc -Wall -Wextra $t/reverse.c -o $t/reverse
prints $'reverse ("cat") = tac\nreverse ("noon") = noon' $t/reverse
must_fail gcc $t/reversefirst.c -o $t/reversefirst
must_fail gcc -c tools/book/reverse.c -o tools/book/reverse.o
must_fail gcc -c tools/book/main2.c -o tools/book/main2.o
t=tools/modules
run gcc "${CFLAGS[@]}" $t/main1.c $t/reverse.c -o $t/main1
run gcc "${CFLAGS[@]}" $t/main2.c $t/reverse.c $t/palindrome.c -o $t/main2
run gcc -std=c17 -Wall -Werror -g -fsanitize=address $t/pal.c $t/reverse.c $t/palindrome.c -o $t/pal
run gcc "${CFLAGS[@]}" -pg $t/main2-prof.c $t/reverse.c $t/palindrome.c -o $t/main2-prof
run gcc "${CFLAGS[@]}" -pg $t/profiling-test.c -o $t/profiling-test

echo "== make (12.2)"
for mk in main1.make main2.make main1-commented.make sleek.make book.make; do
    run make -C make -f $mk
    rm -f make/*.o make/main1 make/main2
done
must_fail make -C make -f spaces.make
must_fail make -C make -f book.make lint
run make -C make
run make -C make/dep

echo "== git (12.3)"
run make -C git/project -f main2.make

if [ "$failed" -ne 0 ]; then echo "Κάποιοι έλεγχοι απέτυχαν."; exit 1; fi
echo "Όλοι οι έλεγχοι πέρασαν."
