#define _DEFAULT_SOURCE  /* for MAP_ANONYMOUS */
#include <semaphore.h>  /* for sem_init, sem_wait, sem_post */
#include <stdbool.h>
#include <stdio.h>      /* for printf, perror */
#include <stdlib.h>     /* for exit */
#include <string.h>     /* for strcmp */
#include <sys/mman.h>   /* for mmap */
#include <sys/wait.h>   /* for wait */
#include <unistd.h>     /* for fork */

#define ROUNDS 1000000

struct shared {
    volatile long counter;  /* read, add 1, write back: three steps */
    sem_t lock;             /* a binary semaphore */
};

int main(int argc, char *argv[]) {
    bool useLock = argc > 1 && strcmp(argv[1], "lock") == 0;

    /* Anonymous shared memory: after fork() both processes see it */
    struct shared *s = mmap(NULL, sizeof *s, PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (s == MAP_FAILED) { perror("mmap"); exit(1); }
    s->counter = 0;
    sem_init(&s->lock, 1, 1);  /* shared by processes, initial value 1 */

    pid_t pid = fork();  /* parent and child run the same loop */
    for (int i = 0; i < ROUNDS; i++) {
        if (useLock) sem_wait(&s->lock);  /* 1 -> 0, or sleep while 0 */
        s->counter++;
        if (useLock) sem_post(&s->lock);  /* 0 -> 1, wake a sleeper */
    }
    if (pid == 0)
        exit(0);
    wait(NULL);
    printf("counter = %ld (expected %d)\n", s->counter, 2 * ROUNDS);
    return 0;
}
