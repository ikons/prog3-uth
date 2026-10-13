#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ  0  /* index of the read end of the pipe */
#define WRITE 1  /* index of the write end of the pipe */

static const char *phrase = "Stuff this in your pipe and smoke it";

int main(void) {
    int fd[2];
    char message[100];  /* parent's message buffer */

    if (pipe(fd) == -1) {  /* create an unnamed pipe */
        perror("pipe");
        return 1;
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {  /* child, writer */
        close(fd[READ]);  /* close unused end */
        write(fd[WRITE], phrase, strlen(phrase) + 1);  /* include '\0' */
        close(fd[WRITE]);  /* close used end */
        return 0;
    }
    close(fd[WRITE]);  /* parent, reader: close unused end */
    ssize_t bytesRead = read(fd[READ], message, sizeof message);
    printf("Read %zd bytes: %s\n", bytesRead, message);
    close(fd[READ]);  /* close used end */
    waitpid(pid, NULL, 0);
    return 0;
}
