/* Two threads add 1 to the same counter a million times each.
 * ./race loses updates; ./race lock protects counter++ with a mutex. */
#include <pthread.h>  /* for pthread_*, pthread_mutex_* */
#include <stdbool.h>
#include <stdio.h>    /* for printf */
#include <string.h>   /* for strcmp */

#define ROUNDS 1000000

long counter = 0;  /* shared by all threads */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
bool useLock;

void *add(void *arg) {
    for (int i = 0; i < ROUNDS; i++) {
        if (useLock) pthread_mutex_lock(&lock);
        counter++;  /* load, add 1, store */
        if (useLock) pthread_mutex_unlock(&lock);
    }
    return arg;
}

int main(int argc, char *argv[]) {
    useLock = argc > 1 && strcmp(argv[1], "lock") == 0;
    pthread_t a, b;
    pthread_create(&a, NULL, add, NULL);
    pthread_create(&b, NULL, add, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    printf("counter = %ld (expected %d)\n", counter, 2 * ROUNDS);
    return 0;
}
