/* REVERSE.C */

#include <stdio.h>

/* Function Prototype */
void reverse ();

/****************************************************************/

main ()

{
  char str [100]; /* Buffer to hold reversed string */

  reverse ("cat", str); /* Reverse the string "cat" */
  printf ("reverse (\"cat\") = %s\n", str); /* Display result */
  reverse ("noon", str); /* Reverse the string "noon" */
  printf ("reverse (\"noon\") = %s\n", str); /* Display result */
}

/****************************************************************/

void reverse (before, after)

char *before; /* A pointer to the source string */
char *after; /* A pointer to the reversed string */

{
  int i;
  int j;
  int len;

  len = strlen (before);

  for (j = len - 1, i = 0; j >= 0; j--, i++) /* Reverse loop */
    after[i] = before[j];

  after[len] = NULL; /* NULL terminate reversed string */
}
