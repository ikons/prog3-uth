#define _POSIX_C_SOURCE 200809L
#define _FILE_OFFSET_BITS 64
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include "io.h"

enum { BUFFER_SIZE = 4096, MAX_LINES = 100000 };
static off_t line_start[MAX_LINES + 1];
static size_t line_count;

static void fail(const char *message)
{
    fprintf(stderr, "reverse: %s\n", message);
    exit(EXIT_FAILURE);
}

static void add_boundary(off_t offset)
{
    if (line_count == MAX_LINES) fail("too many lines");
    line_start[++line_count] = offset;
}

static int pass1(int input, bool spool)
{
    int fd = input;
    if (spool) {
        char name[] = "/tmp/reverse.XXXXXX";
        fd = mkstemp(name);
        if (fd == -1) die("mkstemp");
        if (unlink(name) == -1) die("unlink");
    }
    char buffer[BUFFER_SIZE];
    off_t offset = 0;
    for (;;) {
        ssize_t n = read_retry(input, buffer, sizeof buffer);
        if (n == -1) die("read");
        if (n == 0) break;
        for (ssize_t i = 0; i < n; ++i) {
            ++offset;
            if (buffer[i] == '\n') add_boundary(offset);
        }
        if (spool) write_all(fd, buffer, (size_t)n);
    }
    if (offset > line_start[line_count]) add_boundary(offset);
    return fd;
}

static void reverse_bytes(char *buffer, size_t n)
{
    if (n > 0 && buffer[n - 1] == '\n') --n;
    for (size_t i = 0; i < n / 2; ++i) {
        char tmp = buffer[i];
        buffer[i] = buffer[n - 1 - i];
        buffer[n - 1 - i] = tmp;
    }
}

static void process_line(int fd, size_t i, bool chars)
{
    off_t length = line_start[i + 1] - line_start[i];
    if (length > BUFFER_SIZE) fail("line exceeds 4096 bytes");
    char buffer[BUFFER_SIZE];
    size_t want = (size_t)length, used = 0;
    if (lseek(fd, line_start[i], SEEK_SET) == (off_t)-1)
        die("lseek");
    while (used < want) {
        ssize_t n = read_retry(fd, buffer + used, want - used);
        if (n == -1) die("read");
        if (n == 0) fail("input changed during reading");
        used += (size_t)n;
    }
    if (chars) reverse_bytes(buffer, used);
    write_all(STDOUT_FILENO, buffer, used);
    if (used > 0 && buffer[used - 1] != '\n')
        write_all(STDOUT_FILENO, "\n", 1);
}

int main(int argc, char **argv)
{
    bool chars = false;
    int option;
    while ((option = getopt(argc, argv, "c")) != -1) {
        if (option != 'c') fail("usage: reverse [-c] [file]");
        chars = true;
    }
    if (argc - optind > 1) fail("usage: reverse [-c] [file]");
    bool spool = (optind == argc);
    int input = STDIN_FILENO;
    if (!spool) {
        input = open(argv[optind], O_RDONLY);
        if (input == -1) die("open");
    }
    int fd = pass1(input, spool);
    for (size_t i = line_count; i > 0; --i)
        process_line(fd, i - 1, chars);
    close_checked(fd);
    return EXIT_SUCCESS;
}
