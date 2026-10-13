#define _XOPEN_SOURCE 700
#include <errno.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

int main(void) {
    errno = 0;
    int before = getpriority(PRIO_PROCESS, 0);
    if (before == -1 && errno != 0) {
        perror("getpriority");
        return 1;
    }

    int after = nice(10);
    if (after == -1) {
        perror("nice");
        return 1;
    }
    printf("nice before=%d after=%d\n", before, after);
    return 0;
}
