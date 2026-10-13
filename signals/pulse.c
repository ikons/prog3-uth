#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static pid_t spawn(const char *name) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
        for (;;) {  /* child: infinite loop */
            printf("%s is alive\n", name);
            sleep(1);
        }
    return pid;
}

int main(void) {
    pid_t pid1 = spawn("pid1");
    pid_t pid2 = spawn("pid2");
    sleep(3);
    kill(pid1, SIGSTOP);  /* suspend pid1 */
    sleep(3);
    kill(pid1, SIGCONT);  /* resume pid1 */
    sleep(3);
    kill(pid1, SIGINT);  /* kill both */
    kill(pid2, SIGINT);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}
