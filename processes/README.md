# Διαχείριση διεργασιών

Συνοδευτικός κώδικας της διάλεξης 13.2 «Διαχείριση διεργασιών». Αφετηρία: Glass
& Ables, «UNIX for Programmers and Users», 3η έκδοση, κεφάλαιο 13, έντυπες
σελίδες 472–489. Οι διαφάνειες δείχνουν αποσπάσματα από αυτά ακριβώς τα αρχεία.

Οι καταγραφές της διάλεξης έγιναν στις 27/09/2026 στο περιβάλλον του
[Dockerfile](../Dockerfile), ως `student`, με Debian 13, GCC 15.3 και strace
6.13. Οι αριθμοί PID/PPID, το `ps` και τα δεσμευμένα blocks διαφέρουν σε κάθε
εκτέλεση και σε άλλο περιβάλλον.

## Μεταγλώττιση και εκτέλεση

```bash
for f in *.c; do
    gcc -std=c17 -Wall -Wextra -Werror -g "$f" -o "${f%.c}" || break
done

./myfork
./myexit; echo "exit=$?"
./mywait
./orphan; sleep 3
./zombie & sleep 1; ps -o pid,ppid,stat,comm | grep -E 'PID|zombie'; wait
./myexec
./mychdir
./nice
./identity
./redirect ls.out ls -l myexec.c redirect.c && cat ls.out
./background sleep 5
ps -o pid,ppid,stat,comm --forest
strace -f -e trace=clone,wait4,exit_group -o trace.txt ./myfork
```

Το `zombie.c` αφήνει σκόπιμα μια θυγατρική διεργασία σε κατάσταση ζόμπι για
10 δευτερόλεπτα, χωρίς η πατρική να καλέσει `wait`. Το `orphan.c` αφήνει
σκόπιμα μια ορφανή διεργασία: η πατρική τερματίζει πρώτη, και η θυγατρική
κληρονομεί νέο PPID.

**Ιδιαιτερότητα αυτού του container:** το PID 1 είναι η εντολή εκκίνησης
`sleep infinity` του container, όχι `init` ή `systemd`. Δεν κάνει ποτέ `wait`
για τα ορφανά της, οπότε οι διεργασίες ζόμπι αυτού του κεφαλαίου (και άλλων
πειραμάτων) συσσωρεύονται με PPID 1 αντί να «καθαρίζονται» αμέσως όπως σε ένα
πραγματικό σύστημα init/systemd. Το φαινόμενο είναι πραγματικό και αναφέρεται
στις σημειώσεις της διάλεξης ως παράδειγμα «τι κάνει διαφορετικά ένα σωστό
PID 1».

Το `mywait.c` χρησιμοποιεί `waitpid` και τις μακροεντολές `WIFEXITED` και
`WEXITSTATUS`, αντί για απευθείας ολισθήσεις bit στην ακατέργαστη κατάσταση
του βιβλίου. Το `nice.c` διαβάζει την τρέχουσα προτεραιότητα με `getpriority`,
επειδή το `nice()` επιστρέφει επίσης -1 σε επιτυχία όταν η νέα τιμή είναι -1.
