#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        printf("child %d exits with 42\n", getpid());
        fflush(stdout);
        _exit(42);
    }

    int status = 0;
    pid_t child = waitpid(pid, &status, 0);
    if (child == -1) {
        perror("waitpid");
        return 1;
    }
    if (WIFEXITED(status)) {
        printf("child %d exited normally with %d\n", child, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("child %d was killed by signal %d\n", child, WTERMSIG(status));
    } else {
        printf("child %d stopped or continued\n", child);
    }
    return 0;
}
