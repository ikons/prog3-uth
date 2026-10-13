# Το git

Συνοδευτικός κώδικας της διάλεξης 12.3 «Το git». Οι διαφάνειες δείχνουν
αποσπάσματα από αυτά ακριβώς τα αρχεία. Όλη η ιστορία του έργου της διάλεξης, με
τις διακλαδώσεις και τα pull requests, είναι στο δημόσιο αποθετήριο
<https://github.com/ikons/hello-world>.

| Αρχείο | Τι δείχνει |
|---|---|
| `project/` | η τελική μορφή του έργου: `main2` με τις μονάδες `reverse`, `palindrome` και `removevowel`, το `main2.make` και το `.gitignore` |
| `main2-feature.c` | το `main2.c` της διακλάδωσης `remove-vowels-unicode-support` |

Η διαφάνεια της επίλυσης της διένεξης δείχνει δύο γραμμές του `project/main2.c`.

```bash
cd project && make -f main2.make && ./main2
```

Η `removevowel()` δουλεύει με `wchar_t`, για να αφαιρεί και ελληνικά φωνήεντα·
χρειάζεται locale UTF-8 (π.χ. `LANG=C.UTF-8`).
