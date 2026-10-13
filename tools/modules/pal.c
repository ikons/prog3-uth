/* pal.c: είναι καρκινική η λέξη; */
#include <stdio.h>
#include "palindrome.h"

int main(int argc, char *argv[])
{
    int r = palindrome(argv[1]);

    printf("%s: %d\n", argv[1], r);
    return 0;
}
