# Νήματα

Συνοδευτικός κώδικας της διάλεξης 13.6 «Νήματα». Αφετηρία: τα προγράμματα της
διάλεξης (`threads.c`, `thread_join.c`, `chef-cook-threads`, `thread1.py`) και
Glass & Ables, «UNIX for Programmers and Users», 3η έκδοση, έντυπες σελίδες
486–487. Η θεωρία της διάλεξης είναι από το κεφάλαιο 4 του Stallings. Οι
διαφάνειες δείχνουν αποσπάσματα από αυτά ακριβώς τα αρχεία.

Οι καταγραφές της διάλεξης έγιναν στις 30/09/2026 στο περιβάλλον του
[Dockerfile](../Dockerfile), ως `student`, με Debian 13, GCC 15.3 και glibc
2.41, σε μηχάνημα 20 πυρήνων, με `docker run --security-opt seccomp=unconfined
--hostname unix`. Χωρίς το `seccomp=unconfined`, το προεπιλεγμένο seccomp του
Docker απαντά ENOSYS στη `clone3()` (η glibc ξαναδοκιμάζει με `clone()`),
απαγορεύει την `personality()` που χρειάζεται το ThreadSanitizer, και την
`unshare`. PID, σειρά νημάτων, αριθμοί και χρόνοι διαφέρουν σε κάθε εκτέλεση.

## Μεταγλώττιση

```bash
for f in *.c; do
    gcc -std=c17 -Wall -Wextra -Werror -g -pthread "$f" -o "${f%.c}" || break
done
```

Ο `cook` είναι ο ίδιος με του μέρους 5 (`../sockets/cook.c`).

## Εκτέλεση

Το `session.py` ανοίγει διαδραστικό `bash` σε ψευδοτερματικό και τυπώνει ό,τι θα
έβλεπε ο χρήστης:

```bash
./session.py              # όλες οι καταγραφές της διάλεξης
./session.py race tsan    # μόνο αυτές
```

| Πρόγραμμα | Προέλευση | Τι δείχνει |
|---|---|---|
| `hello.c` | `threads.c` της διάλεξης | τέσσερα νήματα, `pthread_create()` και `pthread_join()` |
| `busywork.c` | `thread_join.c` της διάλεξης | χαρακτηριστικά νήματος, τιμή εξόδου με `pthread_exit()`, παραλληλία (`time`) |
| `args.c` | νέο | το λάθος με το `&i`· `./args fixed` περνά την τιμή |
| `share.c` | νέο | μια καθολική μεταβλητή μετά από `fork()` και μετά από νήμα· PID και TID |
| `chef.c` | `chef-cook-threads` της διάλεξης | ο chef του μέρους 5 με ένα detached νήμα ανά πελάτη |
| `race.c` | νέο | `./race` χάνει ενημερώσεις, `./race lock` όχι (mutex) |
| `prodcons.c` | νέο | παραγωγός–καταναλωτής με mutex και δύο μεταβλητές συνθήκης |
| `bench.c` | νέο | `./bench [N [MB]]`: κόστος `fork()` και `pthread_create()` |
| `tasks.py` | `thread1.py` της διάλεξης | νήματα της Python, για `ps -L` και `/proc/PID/task` |

## Διαφορές από την παλιά διάλεξη

- `-pthread` αντί για `-lpthread`, και `#include` για ό,τι χρησιμοποιείται: το
  `thread_join.c` δεν είχε `<stdlib.h>` και το `thread_example.c` δεν είχε
  `<unistd.h>`, που με GCC 14 και νεότερο είναι σφάλμα μεταγλώττισης.
- Οι έλεγχοι σφαλμάτων τυπώνουν τον αριθμό που επιστρέφει η `pthread_create()`
  (δεν ορίζει `errno`)· διορθώθηκε το «Joing».
- `busywork.c`: ιδιωτική γεννήτρια xorshift ανά νήμα αντί για `random()`, που
  κλειδώνει μια κοινή κατάσταση και σειριοποιεί τα νήματα· η τιμή εξόδου είναι
  δείκτης στη δομή του νήματος με το αποτέλεσμα.
- `chef.c`: ο περιγραφέας περνά κατά τιμή (`intptr_t`) αντί για `malloc()` ενός
  `int`, και ελέγχεται η `pthread_create()`.
- `tasks.py`: μία συνάρτηση για τις δύο εργασίες, και τυπώνει και το TID του
  πυρήνα (`threading.get_native_id()`).
