#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

/* Read one '\0'-terminated line into str from fd.
   Return 0 at the end of input and 1 otherwise. */
static int readLine(int fd, char *str, size_t size) {
    ssize_t n;
    size_t i = 0;
    do {  /* read characters until '\0' or end of input */
        n = read(fd, &str[i], 1);
    } while (n > 0 && str[i] != '\0' && ++i < size - 1);
    str[i] = '\0';
    return n > 0;
}

int main(void) {
    char str[100];
    unlink("aPipe");  /* remove the named pipe if it already exists */
    if (mkfifo("aPipe", 0660) == -1) {  /* create the named pipe */
        perror("mkfifo");
        return 1;
    }
    int fd = open("aPipe", O_RDONLY);  /* open it for reading */
    if (fd == -1) {
        perror("open");
        return 1;
    }
    while (readLine(fd, str, sizeof str))  /* display received messages */
        printf("%s\n", str);
    close(fd);  /* all writers have closed the pipe */
    unlink("aPipe");
    return 0;
}
