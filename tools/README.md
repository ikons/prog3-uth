# Εργαλεία προγραμματισμού σε C

Συνοδευτικός κώδικας της διάλεξης 12.1 «Εργαλεία προγραμματισμού σε C»: Glass &
Ables, «UNIX for Programmers and Users», 3η έκδοση, κεφάλαιο 12. Οι διαφάνειες
δείχνουν αυτά ακριβώς τα αρχεία· οι συγκρίσεις K&R/ANSI και η αρχή του
`reverse.s` μένουν στις διαφάνειες. Δοκιμάστηκαν με GCC 15.3 σε Debian 13.

| Κατάλογος | Αρχεία | Τι δείχνει |
|---|---|---|
| `single/` | `reversefirst.c` | η πρώτη εκδοχή, **με σκόπιμα συντακτικά λάθη**: τα μηνύματα του μεταγλωττιστή |
| `single/` | `reverse.c` | η διορθωμένη εκδοχή σε ένα αρχείο |
| `book/` | `reverse.c`, `main2.c`, `palindrome.h` | ο κώδικας K&R του βιβλίου, όπως ήταν: το GCC 14 και νεότερο τον απορρίπτει |
| `modules/` | `reverse.c/.h`, `main1.c` | η `reverse` ως χωριστή μονάδα |
| `modules/` | `palindrome.c/.h`, `main2.c` | μια δεύτερη μονάδα που χρησιμοποιεί την πρώτη· `libpal.a` και `libpal.so` |
| `modules/` | `pal.c` | για τον `gdb` και το AddressSanitizer· με `-Wextra` προειδοποιεί σκόπιμα για το `argc` |
| `modules/` | `main2-prof.c`, `profiling-test.c` | για τον `gprof` |

```bash
cd single && cc reversefirst.c          # σφάλματα
cc reverse.c -o reverse && ./reverse
cd ../book && cc reverse.c 2>&1 | grep error:
cd ../modules
cc -c reverse.c palindrome.c main2.c && cc reverse.o palindrome.o main2.o -o main2
ar rcs libpal.a reverse.o palindrome.o && cc main2.c -L. -lpal -o main2
cc -g -fsanitize=address reverse.c palindrome.c pal.c -o pal
cc -pg reverse.c palindrome.c main2-prof.c -o main2-prof && ./main2-prof && gprof -b -p main2-prof gmon.out
```

Η `palindrome()` αντιστρέφει τη λέξη σε έναν πίνακα 100 χαρακτήρων και η
`reverse()` δεν ελέγχει το μέγεθός του: το `pal` με μια λέξη 120 χαρακτήρων
είναι το παράδειγμα του AddressSanitizer (`stack-buffer-overflow`).
