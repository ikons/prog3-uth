#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    char message[100];
    int fd;

    snprintf(message, sizeof message, "Hello from PID %d", (int)getpid());
    size_t messageLen = strlen(message) + 1;  /* include the '\0' */
    do {  /* keep trying to open the pipe until it exists */
        fd = open("aPipe", O_WRONLY);  /* open the named pipe for writing */
        if (fd == -1)
            sleep(1);  /* try again in 1 second */
    } while (fd == -1);

    for (int i = 1; i <= 3; i++) {  /* send three messages */
        write(fd, message, messageLen);  /* write a message down the pipe */
        sleep(3);  /* pause a while */
    }
    close(fd);  /* close the pipe descriptor */
    return 0;
}
