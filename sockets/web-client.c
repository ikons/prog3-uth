#define _POSIX_C_SOURCE 200809L
#include <netdb.h>      /* for getaddrinfo */
#include <stdio.h>      /* for snprintf, fwrite */
#include <string.h>     /* for strlen */
#include <sys/socket.h> /* for socket, connect */
#include <unistd.h>     /* for read, write, close */

#define BUFSIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s host port page\n", argv[0]);
        fprintf(stderr, "example: %s localhost 8080 index.html\n", argv[0]);
        return 2;
    }
    const char *host = argv[1], *port = argv[2], *page = argv[3];

    struct addrinfo *result;  /* IPv4 or IPv6, TCP */
    struct addrinfo hints = {.ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM};
    int err = getaddrinfo(host, port, &hints, &result);  /* name and port */
    if (err != 0) { fprintf(stderr, "%s: %s\n", host, gai_strerror(err)); return 1; }
    int fd = -1;
    for (struct addrinfo *rp = result; rp != NULL; rp = rp->ai_next) {  /* try each */
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == -1) continue;
        if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0) break;  /* connected */
        close(fd);
        fd = -1;
    }
    freeaddrinfo(result);
    if (fd == -1) { perror("connect"); return 1; }

    char buffer[BUFSIZE];  /* the request: three lines and a blank line */
    int len = snprintf(buffer, sizeof buffer,
        "GET /%s HTTP/1.1\r\nHost: %s:%s\r\nConnection: close\r\n\r\n",
        page, host, port);
    write(fd, buffer, len);

    ssize_t n;  /* print the whole response, until the server closes */
    while ((n = read(fd, buffer, sizeof buffer)) > 0)
        fwrite(buffer, 1, n, stdout);
    close(fd);
    return n < 0;
}
