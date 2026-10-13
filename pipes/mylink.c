#include <stdio.h>
#include <unistd.h>

int main(void) {
    if (link("original.txt", "another.txt") == -1) {
        perror("mylink");
        return 1;
    }
    return 0;
}
