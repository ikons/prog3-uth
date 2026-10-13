/* main1.c */
#include <stdio.h>
#include "reverse.h"   /* πρότυπο της reverse() */

int main(void)
{
    char str[100];

    reverse("cat", str);    /* ορίζεται αλλού */
    printf("reverse (\"cat\") = %s\n", str);
    reverse("noon", str);
    printf("reverse (\"noon\") = %s\n", str);
    return 0;
}
