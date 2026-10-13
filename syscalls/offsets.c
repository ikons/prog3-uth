#include <fcntl.h>
#include <inttypes.h>
#include "io.h"

int main(void)
{
    int a = open("test.txt", O_RDONLY);
    if (a == -1) die("open a");
    int b = open("test.txt", O_RDONLY);
    if (b == -1) die("open b");
    int c = dup(a);
    if (c == -1) die("dup");
    char buf[2];
    if (read_retry(a, buf, sizeof buf) != 2) {
        fputs("expected two bytes\n", stderr);
        return EXIT_FAILURE;
    }
    off_t pa = lseek(a, 0, SEEK_CUR);
    off_t pb = lseek(b, 0, SEEK_CUR);
    off_t pc = lseek(c, 0, SEEK_CUR);
    if (pa == -1 || pb == -1 || pc == -1) die("lseek");
    printf("fd:     a=%d b=%d c=%d\n", a, b, c);
    printf("offset: a=%jd b=%jd c=%jd\n",
           (intmax_t)pa, (intmax_t)pb, (intmax_t)pc);
    close_checked(a); close_checked(b); close_checked(c);
    return EXIT_SUCCESS;
}
