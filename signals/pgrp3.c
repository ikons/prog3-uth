#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t gotTtin = 0;

static void sigttinHandler(int sig) { (void)sig; gotTtin = 1; }

int main(void) {
    if (fork() == 0) {  /* child */
        struct sigaction sa = { .sa_handler = sigttinHandler };
        sigemptyset(&sa.sa_mask);
        sigaction(SIGTTIN, &sa, NULL);
        setpgid(0, 0);  /* a new, background process group */
        printf("Enter a string: ");
        fflush(stdout);
        char str[100];
        ssize_t n = read(STDIN_FILENO, str, sizeof str);  /* try the terminal */
        if (n == -1 && errno == EINTR && gotTtin)
            printf("Attempted inappropriate read from control terminal\n");
        return 1;
    }
    wait(NULL);  /* parent */
    return 0;
}
