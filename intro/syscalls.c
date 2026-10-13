/* syscalls.c: open, read, write, close (χωρίς ελέγχους σφαλμάτων· βλ. 13.1) */
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int fd = open("notes.txt", O_RDONLY);
    char buf[512];
    ssize_t n = read(fd, buf, sizeof buf);
    write(1, buf, n);   /* 1 = stdout */
    close(fd);
    return 0;
}
