#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ  0
#define WRITE 1

/* ./eof: the reader closes its copy of the write end and sees EOF.
   ./eof keep: it forgets to, and waits for itself forever. */
int main(int argc, char *argv[]) {
    int keep = argc > 1 && strcmp(argv[1], "keep") == 0;
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }
    pid_t pid = fork();
    if (pid == 0) {  /* child, reader */
        if (!keep)
            close(fd[WRITE]);
        char buf[64];
        ssize_t n, total = 0;
        while ((n = read(fd[READ], buf, sizeof buf)) > 0)
            total += n;
        printf("reader: EOF after %zd bytes\n", total);
        return 0;
    }
    close(fd[READ]);  /* parent, writer */
    write(fd[WRITE], "hello\n", 6);
    close(fd[WRITE]);
    printf("writer: closed the write end\n");
    waitpid(pid, NULL, 0);
    return 0;
}
