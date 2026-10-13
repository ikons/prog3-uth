#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>  /* for inet_pton, inet_ntop, htons */
#include <ctype.h>      /* for isdigit */
#include <netdb.h>      /* for getaddrinfo */
#include <netinet/in.h> /* for AF_INET sockets */
#include <stdbool.h>
#include <stdio.h>      /* for printf, scanf */
#include <stdlib.h>     /* for atoi */
#include <string.h>     /* for memset, strcmp */
#include <sys/socket.h> /* for socket, connect */
#include <unistd.h>     /* for read, close, gethostname */

#define DAYTIME_PORT     13  /* standard port number */
#define DEFAULT_PROTOCOL 0

static bool promptForINETAddress(struct in_addr *address);
static bool nameToAddr(const char *name, struct in_addr *address);
static void readTime(int fd);

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : DAYTIME_PORT;
    struct in_addr inetAddress;  /* 32-bit IP address */
    while (promptForINETAddress(&inetAddress)) {  /* until q or end of input */
        struct sockaddr_in serverAddress = {0};           /* zero it all */
        serverAddress.sin_family = AF_INET;               /* use Internet */
        serverAddress.sin_addr = inetAddress;             /* IP */
        serverAddress.sin_port = htons(port);             /* network order */
        int clientFd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
        if (clientFd == -1) { perror("socket"); return 1; }
        if (connect(clientFd, (struct sockaddr *)&serverAddress,
                    sizeof serverAddress) == -1)
            perror("connect");   /* no daytime server there */
        else
            readTime(clientFd);  /* read the time from the server */
        close(clientFd);         /* close the socket */
    }
    return 0;
}

static bool promptForINETAddress(struct in_addr *address) {
    char hostName[100];  /* name from user: numeric or symbolic */
    while (1) {  /* loop until quit or a legal name is entered */
        printf("Host name (q = quit, s = self): ");
        fflush(stdout);
        if (scanf("%99s", hostName) != 1 || strcmp(hostName, "q") == 0)
            return false;  /* quit */
        if (nameToAddr(hostName, address))
            return true;
        printf("Host name not found\n");
    }
}

static bool nameToAddr(const char *name, struct in_addr *address) {
    /* A name that starts with a digit is a numeric A.B.C.D address */
    if (isdigit((unsigned char)name[0]))
        return inet_pton(AF_INET, name, address) == 1;
    char hostName[256];
    if (strcmp(name, "s") == 0) {  /* the local host */
        gethostname(hostName, sizeof hostName);
        printf("Self host name is %s\n", hostName);
        name = hostName;
    }
    struct addrinfo *result;  /* IPv4 only, one entry per address */
    struct addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_STREAM};
    if (getaddrinfo(name, NULL, &hints, &result) != 0) return false;  /* /etc/hosts, DNS */
    *address = ((struct sockaddr_in *)result->ai_addr)->sin_addr;
    freeaddrinfo(result);
    char text[INET_ADDRSTRLEN];  /* display a readable version */
    printf("Internet Address = %s\n", inet_ntop(AF_INET, address, text, sizeof text));
    return true;
}

/* Read one line up to '\n'; drop the '\r' of "\r\n". */
static bool readLine(int fd, char *str, size_t size) {
    size_t i = 0;
    ssize_t n;
    char c;
    while ((n = read(fd, &c, 1)) > 0 && c != '\n')
        if (c != '\r' && i < size - 1)
            str[i++] = c;
    str[i] = '\0';  /* the book forgets this */
    return n > 0 || i > 0;  /* false at end of input */
}

static void readTime(int fd) {
    char str[200];  /* line buffer */
    printf("The time on the target port is ");
    while (readLine(fd, str, sizeof str))  /* until end of input */
        if (str[0] != '\0')                /* skip blank lines */
            printf("%s\n", str);
}
