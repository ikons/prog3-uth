/* main2.c */
#include <stdio.h>
#include "palindrome.h"

int main(void)
{
    printf("palindrome (\"cat\") = %d\n", palindrome("cat"));
    printf("palindrome (\"noon\") = %d\n", palindrome("noon"));
    return 0;
}
