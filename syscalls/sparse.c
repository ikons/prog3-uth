#include <fcntl.h>
#include "io.h"

static int create(const char *path)
{
    int fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);
    if (fd == -1) die("open");
    write_all(fd, "start\n", 6);
    return fd;
}

int main(void)
{
    int fd = create("sparse.bin");
    if (lseek(fd, 60006, SEEK_SET) == (off_t)-1) die("lseek");
    write_all(fd, "end\n", 4);
    close_checked(fd);

    fd = create("normal.bin");
    char zeros[1000] = {0};
    for (int i = 0; i < 60; ++i)
        write_all(fd, zeros, sizeof zeros);
    write_all(fd, "end\n", 4);
    close_checked(fd);
    return EXIT_SUCCESS;
}
