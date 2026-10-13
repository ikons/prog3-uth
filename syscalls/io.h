#ifndef LECTURE_IO_H
#define LECTURE_IO_H
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static inline void die(const char *what)
{
    perror(what);
    exit(EXIT_FAILURE);
}

static inline ssize_t read_retry(int fd, void *buf, size_t count)
{
    ssize_t n;
    do {
        n = read(fd, buf, count);
    } while (n == -1 && errno == EINTR);
    return n;
}

static inline void write_all(int fd, const void *buf, size_t count)
{
    const char *p = buf;
    while (count > 0) {
        ssize_t n = write(fd, p, count);
        if (n == -1 && errno == EINTR) continue;
        if (n == -1) die("write");
        if (n == 0) { errno = EIO; die("write"); }
        p += n;
        count -= (size_t)n;
    }
}

static inline void close_checked(int fd)
{
    if (close(fd) == -1) die("close");
}
#endif
