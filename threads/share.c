/* What is shared: a child process changes its own copy of a global,
 * a thread changes the one and only copy. */
#define _GNU_SOURCE    /* for gettid */
#include <pthread.h>   /* for pthread_create, pthread_join */
#include <stdio.h>     /* for printf, fflush */
#include <sys/wait.h>  /* for wait */
#include <unistd.h>    /* for fork, getpid, gettid */

int counter = 0;  /* global: data segment */

void show(const char *who) {
    printf("%-12s pid %d, tid %d, counter = %d\n",
           who, getpid(), gettid(), counter);
}

void *work(void *arg) {
    counter++;
    show("thread:");
    return arg;
}

int main(void) {
    show("main:");
    fflush(stdout);     /* see the notes */
    if (fork() == 0) {  /* copied memory */
        counter++;
        show("child:");
        return 0;
    }
    wait(NULL);
    show("after wait:");

    pthread_t tid;      /* the same memory */
    pthread_create(&tid, NULL, work, NULL);
    pthread_join(tid, NULL);
    show("after join:");
    return 0;
}
