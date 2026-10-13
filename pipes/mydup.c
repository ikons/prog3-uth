#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    int fd1 = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd1 == -1) {
        perror("open");
        return 1;
    }
    printf("fd1 = %d\n", fd1);
    write(fd1, "what's", 6);
    int fd2 = dup(fd1);  /* make a copy of fd1 */
    printf("fd2 = %d\n", fd2);
    write(fd2, " up", 3);
    close(0);  /* close standard input */
    int fd3 = dup(fd1);  /* make another copy of fd1 */
    printf("fd3 = %d\n", fd3);
    write(0, " doc", 4);
    dup2(fd1, 2);  /* duplicate channel fd1 (3) to channel 2 */
    write(2, "?\n", 2);
    return 0;
}
