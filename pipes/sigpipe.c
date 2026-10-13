#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Write to a pipe that nobody can read any more.
   ./sigpipe: SIGPIPE kills the writer.  ./sigpipe ignore: write fails. */
int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "ignore") == 0)
        signal(SIGPIPE, SIG_IGN);
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }
    close(fd[0]);  /* no reader left */
    printf("writing to a pipe without readers\n");
    fflush(stdout);
    if (write(fd[1], "x", 1) == -1) {
        perror("write");
        return 1;
    }
    return 0;
}
