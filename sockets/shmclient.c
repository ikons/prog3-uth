#define _POSIX_C_SOURCE 200809L
#include <stdio.h>    /* for printf, perror */
#include <stdlib.h>   /* for exit */
#include <sys/shm.h>  /* for shmget, shmat, shmdt */

#define SHM_KEY  5678
#define SHM_SIZE 1000

int main(void) {
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0600);  /* find, do not create */
    if (shmid == -1) { perror("shmget"); exit(1); }

    char *shm = shmat(shmid, NULL, 0);  /* attach, at another address */
    if (shm == (char *)-1) { perror("shmat"); exit(1); }
    printf("mapped to %p\n", (void *)shm);

    printf("read %d bytes: %.48s...\n", SHM_SIZE, shm);
    shm[0] = '*';  /* tell the server that we have read the segment */

    shmdt(shm);    /* detach; the segment stays until IPC_RMID */
    return 0;
}
