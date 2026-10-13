#include <fcntl.h>
#include "io.h"

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    int fd = open("missing.txt", O_RDONLY);
    if (fd == -1) {
        int saved = errno;
        printf("errno = %d\n", saved);
        errno = saved;
        perror("open missing.txt");
    } else close_checked(fd);
    fd = open("/", O_WRONLY);
    if (fd == -1) perror("open /");
    errno = EACCES;
    fd = open("test.txt", O_RDONLY);
    if (fd == -1) die("open test.txt");
    int after = errno;
    printf("open succeeded: fd = %d, errno = %d\n", fd, after);
    close_checked(fd);
    errno = 0;
    perror("perror with errno=0");
    return EXIT_SUCCESS;
}
