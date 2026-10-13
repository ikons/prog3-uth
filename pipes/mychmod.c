#include <stdio.h>
#include <sys/stat.h>

int main(void) {
    if (chmod("test.txt", 0600) == -1) {  /* use an octal encoding */
        perror("mychmod");
        return 1;
    }
    return 0;
}
