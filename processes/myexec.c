#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("process %d before exec\n", getpid());
    fflush(stdout);

    execl("/bin/ls", "ls", "-l", "myexec.c", (char *)NULL);
    perror("execl");
    return 1;
}
