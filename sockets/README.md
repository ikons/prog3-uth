# Υποδοχές και κοινή μνήμη

Συνοδευτικός κώδικας της διάλεξης 13.5 «Υποδοχές και κοινή μνήμη». Αφετηρία:
Glass & Ables, «UNIX for Programmers and Users», 3η έκδοση, κεφάλαιο 13, έντυπες
σελίδες 509–530 για τις υποδοχές, και τα προγράμματα της διάλεξης (διακομιστής
ιστού, κοινή μνήμη). Οι διαφάνειες δείχνουν αποσπάσματα από αυτά ακριβώς τα
αρχεία.

Οι καταγραφές της διάλεξης έγιναν στις 29/09/2026 στο περιβάλλον του
[Dockerfile](../Dockerfile), ως `student`, με Debian 13 και GCC 15.3· το `ss`
και το `nc` είναι από τα `iproute2` και `netcat-openbsd`. Το Docker θέτει
`net.ipv4.ip_unprivileged_port_start=0`, γι' αυτό ο `daytimed` ακούει στη θύρα
13 χωρίς root· σε κανονικό σύστημα δώστε άλλη θύρα (`./daytimed 1313` και
`./inettime 1313`). PID, διευθύνσεις, θύρες και ώρες διαφέρουν σε κάθε εκτέλεση.

## Μεταγλώττιση

```bash
for f in *.c; do
    gcc -std=c17 -Wall -Wextra -Werror -g -pthread "$f" -o "${f%.c}" || break
done
```

## Εκτέλεση

Το `session.py` ανοίγει διαδραστικό `bash` σε ψευδοτερματικό, ξεκινά τους
διακομιστές στο παρασκήνιο, τρέχει τους πελάτες και τυπώνει ό,τι θα έβλεπε ο
χρήστης:

```bash
./session.py              # όλες οι καταγραφές της διάλεξης
./session.py web          # μόνο μία
```

| Πρόγραμμα | Προέλευση | Τι δείχνει |
|---|---|---|
| `chef.c`, `cook.c` | βιβλίο | υποδοχή AF_UNIX `recipe`: `socket`, `bind`, `listen`, `accept`, `connect`, ένα παιδί ανά πελάτη |
| `inettime.c` | βιβλίο | πελάτης TCP για την υπηρεσία daytime (θύρα 13): `getaddrinfo`, `inet_pton`, `htons` |
| `daytimed.c` | απόσπασμα του βιβλίου | επαναληπτικός διακομιστής daytime με `INADDR_ANY` και `SO_REUSEADDR` |
| `web-server.c` | διάλεξη | διακομιστής HTTP στη θύρα 8080: HTML και εικόνες, 404 και 403 |
| `web-client.c` | διάλεξη | `./web-client localhost 8080 index.html`: πελάτης HTTP για IPv4 και IPv6 |
| `index.html`, `picture.png` | διάλεξη | η σελίδα του διακομιστή ιστού |
| `shmserver.c`, `shmclient.c` | διάλεξη | κοινή μνήμη System V: `shmget`, `shmat`, `shmdt`, `shmctl` |
| `race.c` | νέο | `./race` χάνει ενημερώσεις, `./race lock` όχι: σηματοφόρος POSIX σε κοινή μνήμη |

## Διαφορές από το βιβλίο και από την παλιά διάλεξη

- Όλα τα προγράμματα είναι σε ANSI C με `int main`, `socklen_t` και ελέγχους
  σφαλμάτων.
- `cook.c`: νέα υποδοχή σε κάθε προσπάθεια `connect()`, γιατί μετά από αποτυχία η
  κατάσταση της υποδοχής δεν ορίζεται· η `readLine()` συγκρίνει με `'\0'` αντί
  για `NULL` και σέβεται το μέγεθος του buffer.
- `chef.c`: `accept(serverFd, NULL, NULL)`, γιατί οι πελάτες AF_UNIX δεν έχουν
  όνομα· προαιρετική καθυστέρηση (`./chef 3`), όπως το `sleep(5)` της διάλεξης.
- `inettime.c`: `getaddrinfo()`, `inet_pton()` και `inet_ntop()` αντί για
  `gethostbyname()`, `inet_addr()` και `inet_ntoa()`· μία `connect()` αντί για
  αιώνιο βρόχο· η `readLine()` γράφει το `'\0'` που ξεχνά το βιβλίο και πετά το
  `'\r'`.
- `web-server.c`: ενώνει τα `web-server.c` και `web-server-images-support.c` της
  διάλεξης, προσθέτει `Content-Length`, και απορρίπτει με 403 τις διαδρομές με
  `..` ή `//` (το αρχικό πρόγραμμα έστελνε το `/etc/passwd`).
- `shmserver.c`: `volatile sig_atomic_t`, άδειες `0600` αντί για `0666`, και
  γέμισμα με βάση τη θέση αντί για cast δείκτη σε `int`.
