#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>  /* for htonl, htons, inet_ntop */
#include <netinet/in.h> /* for AF_INET sockets */
#include <stdio.h>      /* for perror, fprintf */
#include <stdlib.h>     /* for atoi, exit */
#include <string.h>     /* for memset, strlen */
#include <sys/socket.h> /* for socket, bind, listen, accept */
#include <time.h>       /* for time, localtime, strftime */
#include <unistd.h>     /* for write, close */

#define DAYTIME_PORT     13  /* standard port number */
#define DEFAULT_PROTOCOL 0

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : DAYTIME_PORT;
    int serverFd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);  /* create */
    if (serverFd == -1) { perror("socket"); exit(1); }
    int on = 1;  /* restart at once, despite connections in TIME_WAIT */
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof serverAddress);    /* clear structure */
    serverAddress.sin_family = AF_INET;                 /* Internet domain */
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);  /* accept all */
    serverAddress.sin_port = htons(port);               /* server port number */
    if (bind(serverFd, (struct sockaddr *)&serverAddress, sizeof serverAddress) == -1) {
        perror("bind"); exit(1);
    }
    listen(serverFd, 5);

    while (1) {
        struct sockaddr_in clientAddress;  /* filled in by accept() */
        socklen_t clientLen = sizeof clientAddress;
        int clientFd = accept(serverFd, (struct sockaddr *)&clientAddress, &clientLen);
        if (clientFd == -1) { perror("accept"); continue; }
        char who[INET_ADDRSTRLEN];
        fprintf(stderr, "daytimed: request from %s port %d\n",
                inet_ntop(AF_INET, &clientAddress.sin_addr, who, sizeof who),
                ntohs(clientAddress.sin_port));
        char line[64];  /* one short answer, so no child is needed */
        time_t now = time(NULL);
        strftime(line, sizeof line, "%a %b %e %H:%M:%S %Y\r\n", localtime(&now));
        write(clientFd, line, strlen(line));
        close(clientFd);
    }
}
