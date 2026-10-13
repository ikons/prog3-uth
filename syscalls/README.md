# Κλήσεις συστήματος και αρχεία

Συνοδευτικός κώδικας της διάλεξης 13.1 «Κλήσεις συστήματος και αρχεία».
Αφετηρία: Glass & Ables, «UNIX for Programmers and Users», 3η έκδοση, κεφάλαιο
13, έντυπες σελίδες 431–450. Οι διαφάνειες δείχνουν αποσπάσματα από αυτά ακριβώς
τα αρχεία.

Οι καταγραφές της διάλεξης έγιναν στις 27/09/2026 στο περιβάλλον του
[Dockerfile](../Dockerfile), ως `student`, με Debian 13, GCC 15.3 και strace
6.13. Οι αριθμοί fd, τα μηνύματα σφαλμάτων και τα δεσμευμένα blocks μπορεί να
διαφέρουν σε άλλο περιβάλλον.

## Μεταγλώττιση και εκτέλεση

Αντιγράψτε τα αρχεία σε έναν κατάλογο εργασίας. Στο Linux:

```bash
for f in *.c; do
    gcc -std=c17 -Wall -Wextra -Werror -g "$f" -o "${f%.c}" || break
done
printf 'alpha\nbeta\ngamma\n' > test.txt
./reverse test.txt
./reverse -c test.txt
cat test.txt | ./reverse
./show_errno 2>&1
./offsets
./seek_stdin < test.txt
printf 'abc\n' | ./seek_stdin
./unlink_open
./sparse
stat -c '%n: size=%s blocks=%b unit=%B' *.bin
cmp sparse.bin normal.bin
strace -e trace=read,lseek -P test.txt ./reverse test.txt
```

Για το πείραμα δημιουργίας, το `created.txt` πρέπει αρχικά να απουσιάζει:

```bash
(umask 027; ./create)
stat -c '%a %s %n' created.txt
./create
```

Η δεύτερη εκτέλεση αποτυγχάνει σκόπιμα επειδή χρησιμοποιούμε `O_EXCL`.
Η `seek_stdin` αποτυγχάνει σκόπιμα όταν η stdin είναι διοχέτευση.
Για την καταγραφή του `show_errno`, το `missing.txt` πρέπει να απουσιάζει.

## Συμβόλαιο του reverse

- Χρήση: `./reverse [-c] [file]`. Χωρίς αρχείο διαβάζει stdin, με προσωρινή
  αποθήκευση σε αρχείο από `mkstemp`, του οποίου το όνομα αφαιρείται αμέσως.
- Αντιστρέφει τη σειρά γραμμών. Κάθε γραμμή εξόδου τελειώνει σε newline,
  ακόμη και αν αυτό έλειπε από την τελευταία γραμμή εισόδου.
- Έως 100000 γραμμές και έως 4096 bytes ανά γραμμή, μαζί με τυχόν newline.
  Η υπέρβαση τερματίζει με διαγνωστικό· μπορεί να έχει ήδη γραφτεί μερική έξοδος.
- Το `-c` αντιστρέφει bytes και προορίζεται για ASCII. Χωρίς `-c`, τα bytes
  των γραμμών διατηρούνται και επιτρέπεται UTF-8.
- Η είσοδος πρέπει να μένει σταθερή όσο εκτελείται το πρόγραμμα.
- Οι βοηθητικές συναρτήσεις του `io.h` αφορούν blocking Ε/Ε. Χειρίζονται
  θετικές μερικές μεταφορές και `EINTR`, και τερματίζουν στα άλλα σφάλματα.

Το `sparse.c` παράγει δύο αρχεία με ίδιο περιεχόμενο: `start\n`, 60000 μηδενικά
bytes και `end\n`. Χρησιμοποιεί `O_TRUNC`, οπότε οι επαναλήψεις είναι συγκρίσιμες.
Η μέτρηση χώρου εξαρτάται από το σύστημα αρχείων.

## Επαλήθευση

Το `verify.py` μεταγλωττίζει σε προσωρινό κατάλογο με AddressSanitizer και
UndefinedBehaviorSanitizer και ελέγχει είσοδο από αρχείο/stdin, κενές γραμμές,
τελικό newline, όρια, UTF-8 χωρίς `-c`, επιλογές και τυχαίες εισόδους.

```bash
python3 verify.py
```

Τα 74 αυτά σενάρια πέρασαν στο περιβάλλον της καταγραφής. Ελέγχθηκε επιπλέον
με προσωρινό `LD_PRELOAD` η λειτουργία με αναγνώσεις έως 3 bytes, εγγραφές έως
2 bytes και τεχνητό `EINTR` ανά τέσσερις κλήσεις· το αποτέλεσμα παρέμεινε σωστό.

Σύγχρονη τεκμηρίωση: [open(2)](https://man7.org/linux/man-pages/man2/open.2.html),
[errno(3)](https://man7.org/linux/man-pages/man3/errno.3.html),
[read(2)](https://man7.org/linux/man-pages/man2/read.2.html),
[write(2)](https://man7.org/linux/man-pages/man2/write.2.html),
[lseek(2)](https://man7.org/linux/man-pages/man2/lseek.2.html),
[close(2)](https://man7.org/linux/man-pages/man2/close.2.html),
[unlink(2)](https://man7.org/linux/man-pages/man2/unlink.2.html),
[mkstemp(3)](https://man7.org/linux/man-pages/man3/mkstemp.3.html).
