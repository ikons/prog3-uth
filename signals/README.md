# Σήματα

Συνοδευτικός κώδικας της διάλεξης 13.3 «Σήματα». Αφετηρία: Glass & Ables, «UNIX
for Programmers and Users», 3η έκδοση, κεφάλαιο 13, έντυπες σελίδες 489–502. Οι
διαφάνειες δείχνουν αποσπάσματα από αυτά ακριβώς τα αρχεία.

Οι καταγραφές της διάλεξης έγιναν στις 29/09/2026 στο περιβάλλον του
[Dockerfile](../Dockerfile), ως `student`, με Debian 13, GCC 15.3 και strace
6.13. Οι αριθμοί PID, οι διευθύνσεις στο `strace` και η σειρά των γραμμών στο
`pulse`/`pgrp1`/`pgrp2` διαφέρουν σε κάθε εκτέλεση.

## Μεταγλώττιση

```bash
for f in *.c; do
    gcc -std=c17 -Wall -Wextra -Werror -g "$f" -o "${f%.c}" || break
done
```

## Εκτέλεση

Τα Control-C και Control-Z πρέπει να περάσουν από πραγματικό τερματικό, ώστε ο
οδηγός τερματικού να στείλει το σήμα στην ομάδα προσκηνίου. Το `session.py`
ανοίγει διαδραστικό `bash` σε ψευδοτερματικό, πληκτρολογεί τις εντολές και
πατά τα πλήκτρα στις σωστές στιγμές, και τυπώνει ό,τι θα έβλεπε ο χρήστης:

```bash
./session.py              # όλες οι καταγραφές της διάλεξης
./session.py critical     # μόνο μία
```

Με το χέρι: `./alarm; echo $?`, `./handler`, `./critical` (Control-C στο 1ο και
στο 4ο δευτερόλεπτο), `./mask` (Control-C στο 1ο), `./limit 5 ls *.c`,
`./limit 4 sleep 100`, `./pulse`, `./pgrp1` και `./pgrp2` (Control-C),
`./pgrp3`, `./segv; echo $?`.

| Πρόγραμμα | Βιβλίο | Τι δείχνει |
|---|---|---|
| `alarm.c` | alarm.c | η προεπιλογή του `SIGALRM` (κωδικός 142) |
| `handler.c` | handler.c | `signal()`, `pause()` και σημαία `volatile sig_atomic_t` |
| `critical.c` | critical.c | `SIG_IGN` και επαναφορά του παλιού χειριστή |
| `mask.c` | — | μπλοκάρισμα με `sigprocmask`: το σήμα εκκρεμεί αντί να χαθεί |
| `limit.c` | limit.c | `SIGCHLD`, `SIGALRM`, `sigsuspend()` |
| `pulse.c` | pulse.c | `SIGSTOP`/`SIGCONT` |
| `pgrp1.c`–`pgrp3.c` | pgrp1–3.c | ομάδες διεργασιών, τερματικό ελέγχου, `SIGTTIN` |
| `segv.c` | — | `SIGSEGV` και κωδικός 139 |

## Διαφορές από το βιβλίο

- Όλα τα προγράμματα είναι σε ANSI C με `int main(void)` και ελέγχους σφαλμάτων.
- Κανένας χειριστής δεν καλεί `printf` ή `exit`: θέτουν μια σημαία
  `volatile sig_atomic_t` (ο `handler.c` γράφει και με `write`), και η `main`
  κάνει τη δουλειά. Οι `limit.c` και `pgrp1–3.c` εγκαθιστούν χειριστές με
  `sigaction`.
- Το `limit.c` μπλοκάρει `SIGCHLD`/`SIGALRM` πριν τη `fork` και περιμένει με
  `sigsuspend`, αντί για τη `sleep` του βιβλίου, ώστε να μη χάνεται σήμα.
- Το `pgrp3.c` διαβάζει με `read` χωρίς `SA_RESTART`, για να διακοπεί η ανάγνωση
  από το `SIGTTIN`· το `<sys/termio.h>` του βιβλίου δεν χρειάζεται.
- Το `handler.c` ορίζει `_POSIX_C_SOURCE`, οπότε η `signal()` του glibc έχει τη
  σημασιολογία System V (`SA_RESETHAND`). Με `-D_DEFAULT_SOURCE` παίρνει τη
  σημασιολογία BSD (`SA_RESTART`)· η διαφάνεια «ίδιο όνομα, δύο συμπεριφορές» το
  δείχνει με `strace`.

Το `session.py` επαναφέρει τα `SIGPIPE`/`SIGXFSZ` σε `SIG_DFL` πριν ξεκινήσει το
`bash`: η Python τα αγνοεί, και χωρίς αυτό θα εμφανίζονταν ως αγνοημένα στο
`/proc/PID/status` της καταγραφής.
