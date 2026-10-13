#include <fcntl.h>
#include "io.h"

int main(void)
{
    int fd = open("created.txt", O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd == -1) die("open");
    write_all(fd, "hello\n", 6);
    close_checked(fd);
    return EXIT_SUCCESS;
}
