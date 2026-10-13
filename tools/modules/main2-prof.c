/* main2-prof.c: μόνο για το προφίλ */
#include "palindrome.h"

int main(void)
{
    for (int i = 0; i < 100000000; i++) {
        palindrome("cat");
        palindrome("noon");
    }
    return 0;
}
