#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    sigset_t block, pending;
    sigemptyset(&block);
    sigaddset(&block, SIGINT);
    sigprocmask(SIG_BLOCK, &block, NULL);  /* ^C now waits */
    printf("critical section: ^C is delayed\n");
    sleep(3);
    sigpending(&pending);
    printf("SIGINT pending: %d\n", sigismember(&pending, SIGINT));
    printf("leaving the critical section\n");
    sigprocmask(SIG_UNBLOCK, &block, NULL);  /* delivered here */
    printf("no ^C arrived, bye\n");
    return 0;
}
