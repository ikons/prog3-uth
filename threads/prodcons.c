/* Producer and consumers over a bounded buffer: one mutex, two condition
 * variables. The producer makes 1..ITEMS, then a 0 for each consumer. */
#include <pthread.h>  /* for pthread_*, pthread_mutex_*, pthread_cond_* */
#include <stdint.h>   /* for intptr_t */
#include <stdio.h>    /* for printf */

#define SIZE 4        /* buffer slots */
#define ITEMS 8
#define CONSUMERS 2

int buf[SIZE], in = 0, out = 0, count = 0;  /* circular queue */
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;

void put(int item) {
    pthread_mutex_lock(&m);
    while (count == SIZE)  /* while, not if */
        pthread_cond_wait(&notFull, &m);
    buf[in] = item;
    in = (in + 1) % SIZE; count++;
    pthread_cond_signal(&notEmpty);
    pthread_mutex_unlock(&m);
}

int get(void) {
    pthread_mutex_lock(&m);
    while (count == 0)
        pthread_cond_wait(&notEmpty, &m);
    int item = buf[out];
    out = (out + 1) % SIZE; count--;
    pthread_cond_signal(&notFull);
    pthread_mutex_unlock(&m);
    return item;
}

void *consumer(void *arg) {
    int id = (int) (intptr_t) arg;
    for (int item; (item = get()) != 0; )  /* 0: no more */
        printf("consumer %d got %d\n", id, item);
    return NULL;
}

int main(void) {
    pthread_t c[CONSUMERS];
    for (long i = 0; i < CONSUMERS; i++)
        pthread_create(&c[i], NULL, consumer, (void *) (i + 1));
    for (int item = 1; item <= ITEMS; item++)
        put(item);  /* main is the producer */
    for (int i = 0; i < CONSUMERS; i++)
        put(0);
    for (int i = 0; i < CONSUMERS; i++)
        pthread_join(c[i], NULL);
    return 0;
}
