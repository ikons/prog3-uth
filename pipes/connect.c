#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ  0
#define WRITE 1

/* Run one side of the pipeline: make `end` the standard input or output. */
static pid_t run(const char *prog, int fd[2], int end, int stdFd) {
    pid_t pid = fork();
    if (pid == 0) {
        dup2(fd[end], stdFd);  /* duplicate used end to stdin/stdout */
        close(fd[READ]);       /* close both original ends */
        close(fd[WRITE]);
        execlp(prog, prog, (char *)NULL);
        perror("connect");     /* should never execute */
        _exit(127);
    }
    return pid;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: connect writer reader\n");
        return 2;
    }
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }
    pid_t writer = run(argv[1], fd, WRITE, STDOUT_FILENO);
    pid_t reader = run(argv[2], fd, READ, STDIN_FILENO);
    close(fd[READ]);   /* the parent uses neither end */
    close(fd[WRITE]);  /* without this, the reader never sees EOF */
    waitpid(writer, NULL, 0);
    waitpid(reader, NULL, 0);
    return 0;
}
