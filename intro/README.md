# Τι είναι το Unix

Συνοδευτικός κώδικας της διάλεξης 1 «Τι είναι το Unix;». Οι διαφάνειες δείχνουν
αποσπάσματα από αυτά ακριβώς τα αρχεία.

| Αρχείο | Τι δείχνει |
|---|---|
| `backup.sh` | μια μικρή δέσμη εντολών: αντίγραφο `.bak` κάθε `*.txt` του τρέχοντος καταλόγου |
| `args.c` | τα ορίσματα της γραμμής εντολών (`argc`, `argv`) και ο κωδικός εξόδου |
| `syscalls.c` | οι κλήσεις `open`, `read`, `write`, `close`· η διαφάνεια δείχνει μόνο το σώμα της `main()` |

```bash
gcc -std=c17 -Wall -Wextra args.c -o args && ./args one "two words"
echo hello > notes.txt
gcc -std=c17 -Wall -Wextra syscalls.c -o syscalls && ./syscalls
sh backup.sh
```

Το `syscalls.c` δεν ελέγχει σφάλματα, για να χωρά στη διαφάνεια· οι σωστοί
έλεγχοι είναι στο [`../syscalls/`](../syscalls/README.md) (κεφάλαιο 13.1).
