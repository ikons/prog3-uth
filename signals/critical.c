#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("I can be Control-C'ed\n");
    sleep(3);
    void (*oldHandler)(int) = signal(SIGINT, SIG_IGN);  /* ignore ^C */
    if (oldHandler == SIG_ERR) {
        perror("signal");
        return 1;
    }
    printf("I'm protected from Control-C now\n");
    sleep(3);
    signal(SIGINT, oldHandler);  /* restore */
    printf("I can be Control-C'ed again\n");
    sleep(3);
    printf("Bye!\n");
    return 0;
}
