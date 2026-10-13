#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

static const char *type(mode_t mode) {  /* the S_IS macros take st_mode */
    if (S_ISREG(mode))  return "regular file";
    if (S_ISDIR(mode))  return "directory";
    if (S_ISLNK(mode))  return "symbolic link";
    if (S_ISFIFO(mode)) return "named pipe";
    if (S_ISCHR(mode))  return "character device";
    if (S_ISBLK(mode))  return "block device";
    if (S_ISSOCK(mode)) return "socket";
    return "unknown";
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        struct stat st;
        if (lstat(argv[i], &st) == -1) {  /* a link itself, not its target */
            perror(argv[i]);
            continue;
        }
        printf("%-10s %-16s inode %-8ju links %ju  mode %04o\n", argv[i],
               type(st.st_mode), (uintmax_t)st.st_ino, (uintmax_t)st.st_nlink,
               (unsigned)(st.st_mode & 07777));
    }
    return 0;
}
