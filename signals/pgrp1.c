#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t gotInt = 0;

static void sigintHandler(int sig) { (void)sig; gotInt = 1; }

int main(void) {
    struct sigaction sa = { .sa_handler = sigintHandler };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);  /* handle Control-C */
    pid_t pid = fork();
    printf("%s PID %d PGRP %d waits\n",
           pid == 0 ? "Child" : "Parent", getpid(), getpgid(0));
    pause();  /* wait for a signal */
    if (gotInt)
        printf("Process %d got a SIGINT\n", getpid());
    if (pid > 0)
        waitpid(pid, NULL, 0);
    return 0;
}
