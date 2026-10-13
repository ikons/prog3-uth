# Διοχετεύσεις και αρχεία

Συνοδευτικός κώδικας της διάλεξης 13.4 «Διοχετεύσεις και αρχεία». Αφετηρία:
Glass & Ables, «UNIX for Programmers and Users», 3η έκδοση, κεφάλαιο 13, έντυπες
σελίδες 502–509 για τις διοχετεύσεις και 451–471 για το monitor και τις λοιπές
κλήσεις αρχείων. Οι διαφάνειες δείχνουν αποσπάσματα από αυτά ακριβώς τα αρχεία.

Οι καταγραφές της διάλεξης έγιναν στις 29/09/2026 στο περιβάλλον του
[Dockerfile](../Dockerfile), ως `student`, με Debian 13, GCC 15.3, strace 6.13
και inotify-tools. Για το `mychown` ο `student` είναι μέλος της ομάδας `users`·
και τα δύο τα στήνει το Dockerfile. PID, αριθμοί inode, ώρες και η σειρά των
μηνυμάτων του `reader` διαφέρουν σε κάθε εκτέλεση.

## Μεταγλώττιση

```bash
for f in *.c; do
    gcc -std=c17 -Wall -Wextra -Werror -g "$f" -o "${f%.c}" || break
done
```

## Εκτέλεση

Το `session.py` ανοίγει διαδραστικό `bash` σε ψευδοτερματικό, πληκτρολογεί τις
εντολές (και το Control-D στο τέλος της εισόδου του `cat`) και τυπώνει ό,τι θα
έβλεπε ο χρήστης. Πριν από κάθε καταγραφή φτιάχνει τα αρχεία από τα οποία ξεκινά
(`SETUP`), και κλείνει τους περιγραφείς που κληρονόμησε από την Python, ώστε το
`mydup` να βρει ελεύθερο τον 3:

```bash
./session.py              # όλες οι καταγραφές της διάλεξης
./session.py monitor      # μόνο μία
```

| Πρόγραμμα | Βιβλίο | Τι δείχνει |
|---|---|---|
| `talk.c` | talk.c | `pipe()` και `fork()`: το παιδί στέλνει ένα μήνυμα στον γονέα |
| `connect.c` | connect.c | πώς φτιάχνει το κέλυφος το `ls \| wc`: `dup2()` και `exec()` |
| `eof.c` | — | `./eof keep`: ο αναγνώστης που δεν κλείνει το άκρο εγγραφής δεν βλέπει EOF |
| `sigpipe.c` | — | εγγραφή χωρίς αναγνώστη: `SIGPIPE` (141), ή `EPIPE` με `ignore` |
| `reader.c`, `writer.c` | reader, writer | επώνυμη διοχέτευση (FIFO) `aPipe`: `./reader & ./writer & ./writer &` |
| `monitor.c` | monitor.c | `stat()`, `opendir()`/`readdir()`, ADDED/CHANGED/DELETED |
| `filetype.c` | — | `lstat()` και οι μακροεντολές `S_IS…()` |
| `mychown.c`, `mychmod.c` | mychown, mychmod | `chown()` (`./mychown users`) και `chmod()` στο `test.txt` |
| `mydup.c` | mydup.c | `dup()` και `dup2()`: τέσσερις περιγραφείς, ένα αρχείο |
| `myfcntl.c` | myfcntl.c | `fcntl()` και `O_APPEND` |
| `winsize.c` | — | `ioctl(TIOCGWINSZ)`: το μέγεθος του τερματικού, `ENOTTY` σε διοχέτευση |
| `mylink.c` | mylink.c | `link()`: δύο ονόματα, ένα inode |
| `truncate.c` | truncate.c | `truncate()`: κόψιμο και συμπλήρωση με `'\0'` |
| `pipesize.c` | — | χωρητικότητα διοχέτευσης (64 KiB) και `PIPE_BUF` στο Linux |

## Διαφορές από το βιβλίο

- Όλα τα προγράμματα είναι σε ANSI C με `int main` και ελέγχους σφαλμάτων.
- `connect.c`: δύο θυγατρικές και ένας γονέας που κλείνει και τα δύο άκρα και
  περιμένει και τις δύο, όπως το bash· στο βιβλίο ο γονέας εκτελεί τον εγγραφέα,
  και το κέλυφος μπορεί να τυπώσει το prompt πριν από την έξοδο του `wc`.
- `reader.c`: `mkfifo()` αντί για `mknod(…, SIFIFO, 0)` και `chmod()`· η
  `readLine()` συγκρίνει με `'\0'` αντί για `NULL` και σέβεται το μέγεθος του buffer.
- `monitor.c`: `getopt()`, `opendir()`/`readdir()` αντί για `getdents()`/`lseek()`,
  `snprintf()` με έλεγχο μήκους, `strftime()` αντί για `asctime()`, σύγκριση του
  `st_mtim` με νανοδευτερόλεπτα, σιωπηλή παράλειψη ονομάτων που δεν υπάρχουν ακόμη
  (ENOENT), και `findEntry()` που βρίσκει και όσα προστέθηκαν στην τρέχουσα σάρωση.
- `mychown.c`: `(uid_t)-1` και ομάδα από τη γραμμή εντολών με `getgrnam()` αντί
  για τη σταθερή ταυτότητα 62.
- `mydup.c`: `dup2(fd1, 2)` αντί για `dup2(3, 2)`, και `O_CREAT`.
- `myfcntl.c`: `F_GETFL` και μετά `F_SETFL` με `| O_APPEND`, αντί για
  `F_SETFL` με `O_WRONLY | O_APPEND`.
- Στο δείγμα του monitor το `myFile.txt` αλλάζει με `cat >>`, γιατί μόνο έτσι
  προκύπτουν τα 18 bytes του CHANGED του βιβλίου.
