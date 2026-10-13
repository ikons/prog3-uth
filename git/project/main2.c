/* main2.c: παλίνδρομα και αφαίρεση φωνηέντων */
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "palindrome.h"
#include "removevowel.h"

/* Αλλαγή από το GitHub */
int main(void)
{
    wchar_t str[] = L"Θα βγάλω όλα τα φωνήεντα από αυτή τη συμβολοσειρά.";
    wchar_t orig[sizeof str / sizeof str[0]];

    setlocale(LC_ALL, "");
    wcscpy(orig, str);
    wprintf(L"palindrome (\"cat\") = %ls\n", palindrome("cat") ? L"ναι" : L"όχι");
    wprintf(L"palindrome (\"noon\") = %ls\n", palindrome("noon") ? L"ναι" : L"όχι");
    removevowel(str);
    wprintf(L"removevowel (\"%ls\") = %ls\n", orig, str);
    return 0;
}
