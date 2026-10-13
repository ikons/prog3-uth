#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>

int main(void) {
    if (truncate("file1.txt", 10) == -1)  /* shorter: padded with '\0' */
        perror("file1.txt");
    if (truncate("file2.txt", 10) == -1)  /* longer: cut */
        perror("file2.txt");
    return 0;
}
