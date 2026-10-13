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
    if (pid == 0) {
        _exit(42);
    }

    printf("parent %d keeps child %d as a zombie for 10 seconds\n", getpid(), pid);
    fflush(stdout);
    sleep(10);
    return 0;
}
