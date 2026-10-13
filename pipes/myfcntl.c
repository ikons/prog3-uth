#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    int fd = open("test.txt", O_WRONLY);  /* open file for writing */
    if (fd == -1) {
        perror("open");
        return 1;
    }
    write(fd, "hi there\n", 9);
    lseek(fd, 0, SEEK_SET);  /* seek to beginning of file */
    int flags = fcntl(fd, F_GETFL);        /* the current flags */
    fcntl(fd, F_SETFL, flags | O_APPEND);  /* add APPEND */
    write(fd, " guys\n", 6);
    close(fd);
    return 0;
}
