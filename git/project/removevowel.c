/* removevowel.c */
#include <wctype.h>
#include "removevowel.h"

/* Φωνήεν: λατινικό ή ελληνικό, με ή χωρίς τόνο και διαλυτικά */
static int is_vowel(wchar_t ch)
{
    static const wchar_t vowels[] = L"aeiouáéíóúαάεέηήιίϊΐοόυύϋΰωώ";
    return ch != L'\0' && wcschr(vowels, towlower(ch)) != NULL;
}

/* Αφαιρεί τα φωνήεντα από τη str, επιτόπου */
void removevowel(wchar_t *str)
{
    int j = 0;
    for (int i = 0; str[i] != L'\0'; i++)
        if (!is_vowel(str[i]))
            str[j++] = str[i];
    str[j] = L'\0';
}
