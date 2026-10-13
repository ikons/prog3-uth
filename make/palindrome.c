/* palindrome.c */
#include <string.h>
#include "palindrome.h"
#include "reverse.h"

int palindrome(const char *str)
{
    char reversedStr[100];

    reverse(str, reversedStr);
    return strcmp(str, reversedStr) == 0;
}
