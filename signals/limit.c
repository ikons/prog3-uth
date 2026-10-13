#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t childDone = 0, alarmDone = 0;

static void onChild(int sig) { (void)sig; childDone = 1; }
static void onAlarm(int sig) { (void)sig; alarmDone = 1; }

static void install(int sig, void (*handler)(int)) {
    struct sigaction sa = { .sa_handler = handler };
    sigemptyset(&sa.sa_mask);
    if (sigaction(sig, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: limit seconds command [arg...]\n");
        return 2;
    }
    int delay = atoi(argv[1]);
    sigset_t block, old;
    sigemptyset(&block);
    sigaddset(&block, SIGCHLD);
    sigaddset(&block, SIGALRM);
    sigprocmask(SIG_BLOCK, &block, &old);  /* no signal is lost */
    install(SIGCHLD, onChild);
    install(SIGALRM, onAlarm);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {  /* child */
        sigprocmask(SIG_SETMASK, &old, NULL);
        execvp(argv[2], &argv[2]);
        perror("limit");
        _exit(127);
    }
    alarm(delay);  /* parent */
    while (!childDone && !alarmDone)
        sigsuspend(&old);  /* unblock and wait, atomically */
    if (!childDone) {
        printf("Child %d exceeded limit and is being killed\n", pid);
        kill(pid, SIGINT);
    }
    waitpid(pid, NULL, 0);
    if (childDone)
        printf("Child %d terminated within %d seconds\n", pid, delay);
    return 0;
}
