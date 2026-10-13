/* The lecture's thread_join.c: three threads add up random numbers, and
 * main collects each result with pthread_join(). */
#include <pthread.h>  /* for pthread_* */
#include <stdint.h>   /* for uint64_t */
#include <stdio.h>    /* for printf */
#include <stdlib.h>   /* for exit */

#define NUM_THREADS 3
#define ROUNDS 200000000L

struct task {
    int id;         /* in: the thread number */
    double result;  /* out: written by the thread */
};

void *busyWork(void *arg) {
    struct task *t = arg;
    uint64_t x = 88172645463325252u + t->id;  /* private */
    double sum = 0.0;
    for (long i = 0; i < ROUNDS; i++) {
        x ^= x << 13; x ^= x >> 7; x ^= x << 17;
        sum += (double) (x >> 33);
    }
    t->result = sum;
    printf("Thread %d: result = %e\n", t->id, sum);
    pthread_exit(t);  /* for pthread_join() */
}

int main(void) {
    pthread_t thread[NUM_THREADS];
    struct task task[NUM_THREADS];
    pthread_attr_t attr;

    pthread_attr_init(&attr);  /* joinable is the default */
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for (int t = 0; t < NUM_THREADS; t++) {
        printf("Creating thread %d\n", t);
        task[t].id = t;
        int rc = pthread_create(&thread[t], &attr,
                                busyWork, &task[t]);
        if (rc != 0) { printf("create: %d\n", rc); exit(1); }
    }
    pthread_attr_destroy(&attr);  /* the threads keep a copy */
    for (int t = 0; t < NUM_THREADS; t++) {
        void *status;
        int rc = pthread_join(thread[t], &status);
        if (rc != 0) { printf("join: %d\n", rc); exit(1); }
        struct task *done = status;
        printf("Joined thread %d: result = %e\n",
               done->id, done->result);
    }
    return 0;
}
