/* The chef of part 5 with one thread per client instead of one child
 * process (the lecture's chef-cook-threads). The clients are the same cook. */
#define _POSIX_C_SOURCE 200809L
#include <pthread.h>    /* for pthread_create, pthread_detach */
#include <stdint.h>     /* for intptr_t */
#include <stdio.h>      /* for perror */
#include <stdlib.h>     /* for atoi, exit */
#include <string.h>     /* for strlen, strcpy */
#include <sys/socket.h> /* for socket, bind, listen, accept */
#include <sys/un.h>     /* for AF_UNIX sockets */
#include <unistd.h>     /* for close, unlink, sleep */

#define DEFAULT_PROTOCOL 0

static int delay;  /* seconds each thread lingers; read by all threads */
static void *writeRecipe(void *arg);

int main(int argc, char *argv[]) {
    delay = argc > 1 ? atoi(argv[1]) : 0;

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

    while (1) {  /* loop forever: a new thread per client */
        int clientFd = accept(serverFd, NULL, NULL);
        if (clientFd == -1) { perror("accept"); continue; }
        pthread_t tid;
        void *arg = (void *) (intptr_t) clientFd;
        if (pthread_create(&tid, NULL,
                           writeRecipe, arg) != 0) {
            close(clientFd);
            continue;
        }
        pthread_detach(tid);  /* nobody joins it */
        /* no close(clientFd): the thread uses the same fd */
    }
}

static void *writeRecipe(void *arg) {  /* lines end with '\0' */
    int fd = (int) (intptr_t) arg;  /* passed by value */
    static const char *line1 = "Ψητή προβατίνα στα κάρβουνα";
    static const char *line2 = "ΜΟΝΟ αλάτι και πιπέρι, και το κρέας σιτεμένο.";
    write(fd, line1, strlen(line1) + 1);  /* first line */
    write(fd, line2, strlen(line2) + 1);  /* second line */
    sleep(delay);  /* keep the connection open a while */
    close(fd);     /* the thread closes its client */
    return NULL;
}
