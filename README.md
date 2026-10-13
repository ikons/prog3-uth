# Προγραμματισμός ΙΙΙ: τα παραδείγματα των διαλέξεων

Ο κώδικας των διαλέξεων του μαθήματος Προγραμματισμός ΙΙΙ (Τμήμα Πληροφορικής
και Τηλεπικοινωνιών, Πανεπιστήμιο Θεσσαλίας). Οι διαφάνειες δείχνουν
αποσπάσματα από αυτά ακριβώς τα αρχεία. Κάθε κατάλογος έχει το δικό του README
με ό,τι δείχνει κάθε πρόγραμμα και πώς μεταγλωττίζεται και τρέχει. Οι
διαφάνειες σε PDF βρίσκονται στο [eClass του μαθήματος](https://eclass.uth.gr/modules/document/index.php?course=CS_U_195&openDir=/5f75dd5eyNaf)
και ανοίγουν αφού συνδεθείτε.

```bash
git clone https://github.com/ikons/prog3-uth.git
cd prog3-uth
```

| Κατάλογος | Διάλεξη | Διαφάνειες | Περιεχόμενο |
|---|---|---|---|
| [`intro/`](intro/README.md) | 1. Τι είναι το Unix; | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-01-intro-gr.pdf) | ένα σκριπτ, `argc`/`argv`, οι πρώτες κλήσεις συστήματος |
| [`users/`](users/README.md) | 2. Βοηθήματα του Unix για μη προγραμματιστές | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-02-users-gr.pdf) | ρυθμίσεις του `vi` |
| [`shells/`](shells/README.md) | 4. Τα κελύφη του Unix | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-04-shells-gr.pdf) | τα πρώτα σκριπτ, here documents |
| [`bash/`](bash/README.md) | 8. Το κέλυφος Bourne Again (bash) | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-08-bash-gr.pdf) | 36 σκριπτ: συνθήκες, βρόχοι, συναρτήσεις, πίνακες |
| [`tools/`](tools/README.md) | 12.1 Εργαλεία προγραμματισμού σε C | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-12-1-tools-gr.pdf) | μεταγλώττιση, μονάδες, βιβλιοθήκες, `gdb`, AddressSanitizer, `gprof` |
| [`make/`](make/README.md) | 12.2 Το βοήθημα make | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-12-2-make-gr.pdf) | makefile, από του βιβλίου ως τις αυτόματες εξαρτήσεις |
| [`git/`](git/README.md) | 12.3 Το git | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-12-3-git-gr.pdf) | το έργο της διάλεξης και μια διακλάδωσή του |
| [`syscalls/`](syscalls/README.md) | 13.1 Κλήσεις συστήματος και αρχεία | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-13-1-syscalls-gr.pdf) | `open`, `read`, `write`, `lseek`, `errno` |
| [`processes/`](processes/README.md) | 13.2 Διαχείριση διεργασιών | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-13-2-processes-gr.pdf) | `fork`, `exec`, `wait`, ορφανές και ζόμπι διεργασίες |
| [`signals/`](signals/README.md) | 13.3 Σήματα | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-13-3-signals-gr.pdf) | χειριστές, `alarm`, ομάδες διεργασιών, έλεγχος εργασιών |
| [`pipes/`](pipes/README.md) | 13.4 Διοχετεύσεις και αρχεία | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-13-4-pipes-gr.pdf) | `pipe`, `dup2`, επώνυμες διοχετεύσεις, `stat`, `monitor` |
| [`sockets/`](sockets/README.md) | 13.5 Υποδοχές και κοινή μνήμη | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-13-5-sockets-gr.pdf) | πελάτες και διακομιστές, διακομιστής ιστού, κοινή μνήμη, σηματοφόροι |
| [`threads/`](threads/README.md) | 13.6 Νήματα | [PDF](https://eclass.uth.gr/modules/document/file.php/CS_U_195/%CE%94%CE%B9%CE%B1%CE%BB%CE%AD%CE%BE%CE%B5%CE%B9%CF%82/unix-13-6-threads-gr.pdf) | `pthread_create`, `pthread_join`, mutex, μεταβλητές συνθήκης |

Βιβλίο: Glass & Ables, «UNIX for Programmers and Users», 3η έκδοση. Η θεωρία των
νημάτων είναι από το κεφάλαιο 4 του Stallings, «Λειτουργικά Συστήματα», 9η
έκδοση.

## Το περιβάλλον

Όλα τα παραδείγματα, και οι έξοδοι που δείχνουν οι διαφάνειες, τρέχουν στο
image του [`Dockerfile`](Dockerfile): GCC 15.3 σε Debian 13, με `gdb`,
`strace`, `valgrind` και όσα άλλα εργαλεία χρησιμοποιούν οι διαλέξεις, και
χρήστη `student` χωρίς δικαιώματα root. Φτιάξτε το μία φορά:

```bash
docker build -t prog3 .
```

και ανοίξτε ένα κέλυφος με τον κατάλογο `prog3-uth` στο `~/examples`:

```bash
docker run --rm -it --init --hostname unix -v "$PWD":/home/student/examples prog3
```

Στο PowerShell των Windows γράψτε `"${PWD}:/home/student/examples"`. Ό,τι
αλλάζετε στο `~/examples` αλλάζει και στον δικό σας δίσκο, οπότε μπορείτε να
γράφετε κώδικα με τον επεξεργαστή σας και να τον μεταγλωττίζετε στο container.

- **Linux:** αν ο χρήστης σας δεν έχει UID 1000, φτιάξτε το image με
  `docker build --build-arg STUDENT_UID="$(id -u)" -t prog3 .`, αλλιώς ο
  `student` δεν θα μπορεί να γράψει στον κατάλογο.
- **ThreadSanitizer, `unshare`:** το προεπιλεγμένο seccomp του Docker τα
  απαγορεύει. Για τις ασκήσεις αυτές προσθέστε
  `--security-opt seccomp=unconfined` στο `docker run`.
- **Χωρίς Docker:** σε Debian 13 ή Ubuntu 24.04 και νεότερο αρκούν τα πακέτα
  του Dockerfile (`sudo apt install build-essential gdb strace valgrind …`).
  Με παλαιότερο GCC μερικά μηνύματα διαφέρουν από τις διαφάνειες: για
  παράδειγμα, πριν από το GCC 14 ο κώδικας K&R του βιβλίου μεταγλωττίζεται με
  προειδοποιήσεις αντί για σφάλματα.

## Έλεγχος

Το [`check.sh`](check.sh) μεταγλωττίζει όλα τα παραδείγματα όπως γράφουν τα
README, σε ένα προσωρινό αντίγραφο, ελέγχει τη σύνταξη των σκριπτ και ότι τα
σκόπιμα λάθη των διαλέξεων παραμένουν λάθη:

```bash
docker run --rm -v "$PWD":/home/student/examples prog3 ./check.sh
```
