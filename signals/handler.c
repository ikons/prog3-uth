#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static volatile sig_atomic_t alarmFlag = 0;

static void alarmHandler(int sig) {
    (void)sig;
    static const char msg[] = "An alarm clock signal was received\n";
    write(STDOUT_FILENO, msg, sizeof msg - 1);  /* not printf */
    alarmFlag = 1;
}

int main(void) {
    if (signal(SIGALRM, alarmHandler) == SIG_ERR) {
        perror("signal");
        return 1;
    }
    alarm(3);
    printf("Looping...\n");
    while (!alarmFlag)
        pause();  /* sleep until a signal arrives */
    printf("Loop ends due to alarm signal\n");
    return 0;
}
