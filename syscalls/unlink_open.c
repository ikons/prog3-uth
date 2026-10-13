#define _POSIX_C_SOURCE 200809L
#include "io.h"

int main(void)
{
    char name[] = "/tmp/unlink-demo.XXXXXX";
    int fd = mkstemp(name);
    if (fd == -1) die("mkstemp");
    write_all(fd, "still here\n", 11);
    if (unlink(name) == -1) die("unlink");
    if (lseek(fd, 0, SEEK_SET) == (off_t)-1) die("lseek");
    char buffer[32];
    ssize_t n = read_retry(fd, buffer, sizeof buffer);
    if (n == -1) die("read");
    write_all(STDOUT_FILENO, buffer, (size_t)n);
    close_checked(fd);
    return EXIT_SUCCESS;
}
