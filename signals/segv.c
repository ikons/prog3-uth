#include <stdio.h>

int main(void) {
    volatile int *p = NULL;
    printf("about to dereference NULL\n");
    return *p;  /* SIGSEGV */
}
