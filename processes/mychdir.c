#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void print_cwd(void) {
    char cwd[4096];
    if (getcwd(cwd, sizeof cwd) == NULL) {
        perror("getcwd");
        exit(1);
    }
    puts(cwd);
}

int main(void) {
    print_cwd();
    if (chdir("/") == -1) {
        perror("chdir");
        return 1;
    }
    print_cwd();
    return 0;
}
