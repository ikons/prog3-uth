#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static volatile sig_atomic_t gotInt = 0;

static void sigintHandler(int sig) { (void)sig; gotInt = 1; }

int main(void) {
    struct sigaction sa = { .sa_handler = sigintHandler };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    if (fork() == 0)
        setpgid(0, 0);  /* child: its own process group */
    printf("Process PID %d PGRP %d waits\n", getpid(), getpgid(0));
    for (int i = 1; i <= 3 && !gotInt; i++) {
        printf("Process %d is alive\n", getpid());
        sleep(1);  /* a signal cuts the sleep short */
    }
    if (gotInt) {
        printf("Process %d got a SIGINT\n", getpid());
        return 1;
    }
    return 0;
}
