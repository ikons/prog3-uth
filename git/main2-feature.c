/* main2.c */
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "palindrome.h"
#include "removevowel.h"

int main(void)
{
    wchar_t str[] = L"Θα βγάλω όλα τα φωνήεντα από αυτή τη συμβολοσειρά.";
    wchar_t orig[sizeof str / sizeof str[0]];

    setlocale(LC_ALL, "");
    wcscpy(orig, str);
    wprintf(L"palindrome (\"cat\") = %d\n", palindrome("cat"));
    wprintf(L"palindrome (\"noon\") = %d\n", palindrome("noon"));
    removevowel(str);
    wprintf(L"removevowel (\"%ls\") = %ls\n", orig, str);
    return 0;
}
