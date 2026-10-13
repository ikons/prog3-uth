#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid != 0) {
        printf("parent %d exits; child %d remains\n", getpid(), pid);
        fflush(stdout);
        _exit(0);
    }

    sleep(2);
    printf("orphan %d now has ppid %d\n", getpid(), getppid());
    return 0;
}
