/* A classic bug: every thread gets the address of the same loop variable.
 * ./args shows it; ./args fixed passes each thread its own value. */
#include <pthread.h>  /* for pthread_* */
#include <stdint.h>   /* for intptr_t */
#include <stdio.h>    /* for printf */

#define NUM_THREADS 4

void *byAddress(void *arg) {  /* reads i whenever it runs */
    printf("thread %d\n", *(int *) arg);
    return NULL;
}

void *byValue(void *arg) {    /* a copy, taken at create */
    printf("thread %d\n", (int) (intptr_t) arg);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread[NUM_THREADS];
    int i;
    for (i = 0; i < NUM_THREADS; i++) {
        if (argc > 1 && argv[1][0] == 'f')  /* fixed */
            pthread_create(&thread[i], NULL, byValue,
                           (void *) (intptr_t) i);
        else  /* BUG: all four get &i */
            pthread_create(&thread[i], NULL, byAddress, &i);
    }
    for (int j = 0; j < NUM_THREADS; j++)
        pthread_join(thread[j], NULL);
    return 0;
}
