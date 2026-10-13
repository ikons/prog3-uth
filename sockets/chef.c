#define _POSIX_C_SOURCE 200809L
#include <signal.h>     /* for signal */
#include <stdio.h>      /* for perror */
#include <stdlib.h>     /* for atoi, exit */
#include <string.h>     /* for strlen, strcpy */
#include <sys/socket.h> /* for socket, bind, listen, accept */
#include <sys/un.h>     /* for AF_UNIX sockets */
#include <unistd.h>     /* for fork, close, unlink, sleep */

#define DEFAULT_PROTOCOL 0

static void writeRecipe(int fd);

int main(int argc, char *argv[]) {
    int delay = argc > 1 ? atoi(argv[1]) : 0;  /* seconds each child lingers */
    signal(SIGCHLD, SIG_IGN);  /* ignore death-of-child signals: no zombies */

    /* Create a UNIX socket, bidirectional, default protocol */
    int serverFd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
    if (serverFd == -1) { perror("socket"); exit(1); }
    struct sockaddr_un serverAddress = {0};     /* server address */
    serverAddress.sun_family = AF_UNIX;         /* set domain type */
    strcpy(serverAddress.sun_path, "recipe");   /* set name */
    unlink("recipe");                           /* remove file if it exists */
    if (bind(serverFd, (struct sockaddr *)&serverAddress,
             sizeof serverAddress) == -1) {      /* create the file */
        perror("bind"); exit(1);
    }
    listen(serverFd, 5);  /* maximum pending connection length */

    while (1) {  /* loop forever */
        int clientFd = accept(serverFd, NULL, NULL);  /* AF_UNIX clients have no name */
        if (clientFd == -1) { perror("accept"); continue; }
        if (fork() == 0) {         /* create child to send recipe */
            close(serverFd);       /* the child does not accept */
            writeRecipe(clientFd); /* send the recipe */
            sleep(delay);          /* keep the connection open a while */
            close(clientFd);       /* close the socket */
            exit(0);               /* terminate */
        }
        close(clientFd);  /* the parent closes the client descriptor */
    }
}

static void writeRecipe(int fd) {  /* each line ends with '\0' */
    static const char *line1 = "Ψητή προβατίνα στα κάρβουνα";
    static const char *line2 = "ΜΟΝΟ αλάτι και πιπέρι, και το κρέας σιτεμένο.";
    write(fd, line1, strlen(line1) + 1);  /* write first line */
    write(fd, line2, strlen(line2) + 1);  /* write second line */
}
