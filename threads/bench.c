/* How much does a thread cost compared to a process? Creates and waits for
 * N processes, then N threads, and prints the average time of each.
 * ./bench N MB first fills MB megabytes: fork() must copy their page tables. */
#include <pthread.h>   /* for pthread_create, pthread_join */
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for atoi, malloc */
#include <string.h>    /* for memset */
#include <sys/wait.h>  /* for waitpid */
#include <time.h>      /* for clock_gettime */
#include <unistd.h>    /* for fork, _exit */

static double now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static void *nothing(void *arg) { return arg; }

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 10000;
    int mb = argc > 2 ? atoi(argv[2]) : 0;
    if (mb > 0) memset(malloc(mb << 20), 1, mb << 20);  /* touch every page */

    double t0 = now();
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) _exit(0);  /* the child does nothing */
        waitpid(pid, NULL, 0);
    }
    double proc = (now() - t0) / n * 1e6;

    t0 = now();
    for (int i = 0; i < n; i++) {
        pthread_t t;
        pthread_create(&t, NULL, nothing, NULL);  /* the thread does nothing */
        pthread_join(t, NULL);
    }
    double thr = (now() - t0) / n * 1e6;

    printf("fork + waitpid:              %6.1f us\n", proc);
    printf("pthread_create + join:       %6.1f us\n", thr);
    printf("a thread is %.1f times cheaper\n", proc / thr);
    return 0;
}
