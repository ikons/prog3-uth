#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    printf("original: pid=%d ppid=%d\n", getpid(), getppid());
    fflush(stdout);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        printf("child:    pid=%d ppid=%d\n", getpid(), getppid());
        fflush(stdout);
        _exit(0);
    }

    printf("parent:   pid=%d child=%d\n", getpid(), pid);
    int status = 0;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return 1;
    }
    if (WIFEXITED(status)) {
        printf("child %d exited with %d\n", pid, WEXITSTATUS(status));
    } else {
        printf("child %d did not exit normally\n", pid);
    }
    return 0;
}
