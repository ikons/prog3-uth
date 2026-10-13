/* The lecture's threads.c: the main thread creates four threads, each prints
 * a message, and main waits for all of them with pthread_join(). */
#include <pthread.h>  /* for pthread_create, pthread_join */
#include <stdio.h>    /* for printf, fprintf */
#include <string.h>   /* for strerror */

#define NUM_THREADS 4

void *hello(void *arg) {
    int id = *(int *) arg;  /* its slot */
    printf("Hello from thread %d!\n", id);
    return NULL;  /* = pthread_exit(NULL) */
}

int main(void) {
    pthread_t thread[NUM_THREADS];
    int arg[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        arg[i] = i;
        int err = pthread_create(&thread[i], NULL,
                                 hello, &arg[i]);
        if (err != 0) {  /* not -1 */
            fprintf(stderr, "pthread_create: %s\n",
                    strerror(err));
            return 1;
        }
    }
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(thread[i], NULL);
    printf("All %d threads finished\n", NUM_THREADS);
    return 0;
}
