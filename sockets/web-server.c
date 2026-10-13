#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>  /* for htonl, htons */
#include <stdio.h>      /* for printf, snprintf, fopen, fread */
#include <stdlib.h>     /* for atoi, exit */
#include <string.h>     /* for strtok, strcmp, strstr, strrchr */
#include <sys/socket.h> /* for socket, bind, listen, accept */
#include <sys/stat.h>   /* for fstat */
#include <sys/wait.h>   /* for waitpid */
#include <unistd.h>     /* for fork, read, write, close */

#define DEFAULT_PORT 8080
#define BUFSIZE      4096

static const char *mimeType(const char *fileName) {  /* from the extension */
    const char *ext = strrchr(fileName, '.');
    if (ext == NULL) return "application/octet-stream";
    if (strcmp(ext, ".html") == 0) return "text/html; charset=utf-8";
    if (strcmp(ext, ".txt") == 0) return "text/plain; charset=utf-8";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    return "application/octet-stream";
}

static void sendError(int fd, const char *status) {  /* e.g. "404 Not Found" */
    char body[128], header[256];
    int bodyLen = snprintf(body, sizeof body, "<h1>%s</h1>\n", status);
    int headerLen = snprintf(header, sizeof header,
        "HTTP/1.1 %s\r\nContent-Type: text/html\r\n"
        "Content-Length: %d\r\nConnection: close\r\n\r\n", status, bodyLen);
    write(fd, header, headerLen);
    write(fd, body, bodyLen);
}

static void handleClient(int fd) {
    char buffer[BUFSIZE];
    ssize_t n = read(fd, buffer, sizeof buffer - 1);  /* GET /page HTTP/1.1 ... */
    if (n <= 0) return;
    buffer[n] = '\0';
    char *method = strtok(buffer, " ");
    char *path = strtok(NULL, " ");
    if (method == NULL || path == NULL) { sendError(fd, "400 Bad Request"); return; }
    printf("[%d] %s %s\n", (int)getpid(), method, path);

    if (strcmp(path, "/") == 0)
        path = "/index.html";  /* the default page */
    if (strstr(path, "..") != NULL || path[1] == '/') {
        sendError(fd, "403 Forbidden");  /* only files below this directory */
        return;
    }
    FILE *file = fopen(path + 1, "rb");  /* skip the leading '/' */
    struct stat st;
    if (file == NULL || fstat(fileno(file), &st) == -1 || !S_ISREG(st.st_mode)) {
        if (file != NULL) fclose(file);
        sendError(fd, "404 Not Found");
        return;
    }
    int headerLen = snprintf(buffer, sizeof buffer,
        "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n"
        "Content-Length: %lld\r\nConnection: close\r\n\r\n",
        mimeType(path), (long long)st.st_size);
    write(fd, buffer, headerLen);  /* the header, then the file as is */
    size_t got;
    while ((got = fread(buffer, 1, sizeof buffer, file)) > 0)
        write(fd, buffer, got);
    fclose(file);
}

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : DEFAULT_PORT;
    setvbuf(stdout, NULL, _IOLBF, 0);  /* one log line at a time */

    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) { perror("socket"); exit(1); }
    int on = 1;  /* allow a restart while old connections linger */
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);

    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);  /* every local address */
    address.sin_port = htons(port);
    if (bind(serverFd, (struct sockaddr *)&address, sizeof address) == -1) {
        perror("bind"); exit(1);
    }
    listen(serverFd, 10);
    printf("Server running on port %d\n", port);

    while (1) {
        int clientFd = accept(serverFd, NULL, NULL);  /* no client address */
        if (clientFd == -1) { perror("accept"); continue; }
        if (fork() == 0) {  /* the child serves this client */
            close(serverFd);
            handleClient(clientFd);
            close(clientFd);
            exit(0);
        }
        close(clientFd);  /* the parent goes back to accept(), */
        while (waitpid(-1, NULL, WNOHANG) > 0)
            ;             /* after reaping the children that have finished */
    }
}
