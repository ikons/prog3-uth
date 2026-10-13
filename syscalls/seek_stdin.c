#include "io.h"

int main(void)
{
    if (lseek(STDIN_FILENO, 0, SEEK_SET) == (off_t)-1)
        die("lseek stdin");
    puts("stdin is seekable");
    return EXIT_SUCCESS;
}
