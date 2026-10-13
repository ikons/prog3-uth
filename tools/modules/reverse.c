/* reverse.c */
#include <string.h>
#include "reverse.h"

void reverse(const char *before, char *after)
{
    int i, j, len;

    len = strlen(before);
    for (j = len - 1, i = 0; j >= 0; j--, i++)
        after[i] = before[j];   /* αντιστροφή */
    after[len] = '\0';          /* τερματισμός */
}
