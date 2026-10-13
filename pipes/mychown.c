#define _POSIX_C_SOURCE 200809L
#include <grp.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: mychown group\n");
        return 2;
    }
    struct group *g = getgrnam(argv[1]);  /* group name to group ID */
    if (g == NULL) {
        fprintf(stderr, "mychown: no group %s\n", argv[1]);
        return 1;
    }
    if (chown("test.txt", (uid_t)-1, g->gr_gid) == -1) {  /* keep the owner */
        perror("mychown");
        return 1;
    }
    return 0;
}
