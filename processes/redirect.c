#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s output command [argument...]\n", argv[0]);
        return 2;
    }

    int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0600);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        close(fd);
        return 1;
    }
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    execvp(argv[2], &argv[2]);
    perror("execvp");
    return 127;
}
