#define _DEFAULT_SOURCE  /* for TIOCGWINSZ */
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(void) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {  /* terminals only */
        perror("ioctl");
        return 1;
    }
    printf("%d rows, %d columns\n", ws.ws_row, ws.ws_col);
    return 0;
}
