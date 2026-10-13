#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>      /* for printf, perror */
#include <stdlib.h>     /* for exit */
#include <string.h>     /* for strcpy */
#include <sys/socket.h> /* for socket, connect */
#include <sys/un.h>     /* for AF_UNIX sockets */
#include <unistd.h>     /* for read, close, sleep */

#define DEFAULT_PROTOCOL 0

static void readRecipe(int fd);

int main(void) {
    struct sockaddr_un serverAddress = {0};
    serverAddress.sun_family = AF_UNIX;        /* server domain */
    strcpy(serverAddress.sun_path, "recipe");  /* server name */

    int clientFd;
    while (1) {  /* until a connection is made with the server */
        clientFd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
        if (clientFd == -1) { perror("socket"); exit(1); }
        if (connect(clientFd, (struct sockaddr *)&serverAddress,
                    sizeof serverAddress) == 0)
            break;
        close(clientFd);  /* start again with a new socket */
        sleep(1);         /* wait and then try again */
    }
    readRecipe(clientFd);  /* read the recipe */
    close(clientFd);       /* close the socket */
    return 0;              /* done */
}

/* Read one '\0'-terminated line, keeping at most size - 1 bytes. */
static bool readLine(int fd, char *str, size_t size) {
    size_t i = 0;
    ssize_t n;
    while ((n = read(fd, &str[i], 1)) > 0 && str[i] != '\0')
        if (i < size - 1)
            i++;  /* else drop the rest of a long line */
    str[i] = '\0';
    return n > 0;  /* false at end of input */
}

static void readRecipe(int fd) {
    char str[200];
    while (readLine(fd, str, sizeof str))  /* until end of input */
        printf("%s\n", str);               /* echo the line */
}
