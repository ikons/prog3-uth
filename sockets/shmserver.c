#define _POSIX_C_SOURCE 200809L
#include <signal.h>   /* for signal */
#include <stdio.h>    /* for printf, perror */
#include <stdlib.h>   /* for exit */
#include <sys/shm.h>  /* for shmget, shmat, shmdt, shmctl */
#include <unistd.h>   /* for sleep */

#define SHM_KEY  5678  /* both programs agree on the key */
#define SHM_SIZE 1000

static volatile sig_atomic_t stop = 0;
static void onInterrupt(int sig) { (void)sig; stop = 1; }

int main(void) {
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0600);
    if (shmid == -1) { perror("shmget"); exit(1); }
    printf("shmid=%d\n", shmid);
    char *shm = shmat(shmid, NULL, 0);  /* attach to our address space */
    if (shm == (char *)-1) { perror("shmat"); exit(1); }
    printf("mapped to %p\n", (void *)shm);
    signal(SIGINT, onInterrupt);

    for (int i = 0; i < SHM_SIZE; i++)  /* put something there to read */
        shm[i] = 'a' + i % 16;
    while (!stop && shm[0] != '*')  /* until the client reads it */
        sleep(1);

    printf("cleaning memory\n");
    shmdt(shm);                     /* detach */
    shmctl(shmid, IPC_RMID, NULL);  /* and remove the segment */
    return 0;
}
