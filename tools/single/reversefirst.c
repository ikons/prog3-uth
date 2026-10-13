/* reversefirst.c: η πρώτη έκδοση */
#include <stdio.h>
#include <string.h>

void reverse(const char *before, char *after);

int main(void)
{
    char str[100];  /* χώρος για το αποτέλεσμα */

    reverse("cat", str);
    printf("reverse ("cat") = %s\n", str);
    reverse("noon", str);
    printf("reverse ("noon") = %s\n", str);
    return 0;
}

void reverse(const char *before, char *after)
{
    int i, j, len;

    len = strlen(before);
    for (j = len - 1; i = 0; j >= 0; j--; i++)
        after[i] = before[j];
    after[len] = '\0';  /* τερματισμός */
}
