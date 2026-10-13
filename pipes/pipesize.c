#define _GNU_SOURCE  /* for F_GETPIPE_SZ, Linux only */
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

/* Fill a pipe that nobody reads, without blocking, and count the bytes. */
int main(void) {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }
    fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);
    long total = 0;
    while (write(fd[1], "x", 1) == 1)
        total++;
    if (errno == EAGAIN)
        printf("pipe full after %ld bytes\n", total);
    printf("F_GETPIPE_SZ: %d bytes\n", fcntl(fd[1], F_GETPIPE_SZ));
    printf("PIPE_BUF:     %d bytes (atomic writes)\n", PIPE_BUF);
    return 0;
}
