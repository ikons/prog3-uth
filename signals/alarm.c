#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>

int main(void) {
    alarm(3);  /* SIGALRM in three seconds */
    printf("Looping forever...\n");
    for (;;)
        ;  /* busy loop: burns CPU */
    printf("This line should never be executed\n");
}
