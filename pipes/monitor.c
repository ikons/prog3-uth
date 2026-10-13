#define _POSIX_C_SOURCE 200809L
#include <dirent.h>     /* for opendir, readdir */
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>      /* for printf, fprintf */
#include <stdlib.h>     /* for strtol, exit */
#include <string.h>     /* for strcmp */
#include <sys/stat.h>   /* for stat and the S_IS macros */
#include <time.h>       /* for localtime, strftime */
#include <unistd.h>     /* for getopt, sleep */

#define MAX_FILES           100
#define MAX_FILENAME        256
#define NOT_FOUND           (-1)
#define FOREVER             (-1)
#define DEFAULT_DELAY_TIME  10
#define DEFAULT_LOOP_COUNT  FOREVER

/* Status structure, one per file. */
struct statStruct {
    char fileName[MAX_FILENAME];  /* file name */
    bool lastCycle, thisCycle;    /* to detect changes */
    struct stat status;           /* information from stat() */
};

static char *fileNames[MAX_FILES];     /* one per file on command line */
static int fileCount;                  /* count of files on command line */
static struct statStruct stats[MAX_FILES];  /* one per matching file */
static int loopCount = DEFAULT_LOOP_COUNT;  /* number of times to loop */
static int delayTime = DEFAULT_DELAY_TIME;  /* seconds between loops */

static void monitorFile(const char *fileName);

static void usageError(void) {
    fprintf(stderr, "usage: monitor [-t delay] [-l count] {fileName}+\n");
    exit(2);
}

static int getNumber(const char *str) {  /* a positive number or usage */
    char *end;
    long n = strtol(str, &end, 10);
    if (*str == '\0' || *end != '\0' || n <= 0 || n > 86400)
        usageError();
    return (int)n;
}

static void parseCommandLine(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "t:l:")) != -1) {
        switch (opt) {
        case 't': delayTime = getNumber(optarg); break;
        case 'l': loopCount = getNumber(optarg); break;
        default:  usageError();
        }
    }
    for (int i = optind; i < argc && fileCount < MAX_FILES; i++)
        fileNames[fileCount++] = argv[i];
    if (fileCount == 0)
        usageError();
}

static void printEntry(int index) {  /* display an entry of the table */
    const struct stat *st = &stats[index].status;
    char when[64];
    strftime(when, sizeof when, "%a %b %e %H:%M:%S %Y",
             localtime(&st->st_mtime));
    printf("%s size %lld bytes, mod. time = %s\n",
           stats[index].fileName, (long long)st->st_size, when);
}

static bool inUse(int i) { return stats[i].lastCycle || stats[i].thisCycle; }

static int findEntry(const char *fileName) {  /* index of a file, if any */
    for (int i = 0; i < MAX_FILES; i++)
        if (inUse(i) && strcmp(stats[i].fileName, fileName) == 0)
            return i;
    return NOT_FOUND;
}

static int nextFree(void) {  /* the next free index in the table */
    for (int i = 0; i < MAX_FILES; i++)
        if (!inUse(i))
            return i;
    return NOT_FOUND;
}

static int addEntry(const char *fileName, const struct stat *statBuf) {
    int index = nextFree();
    if (index == NOT_FOUND)
        return NOT_FOUND;  /* none left */
    snprintf(stats[index].fileName, MAX_FILENAME, "%s", fileName);
    stats[index].status = *statBuf;  /* add status information */
    printf("ADDED ");
    printEntry(index);
    return index;
}

static bool modified(const struct stat *a, const struct stat *b) {
    return a->st_mtim.tv_sec != b->st_mtim.tv_sec
        || a->st_mtim.tv_nsec != b->st_mtim.tv_nsec;
}

static void updateEntry(int index, const struct stat *statBuf) {
    if (modified(&stats[index].status, statBuf)) {
        stats[index].status = *statBuf;  /* store stat information */
        printf("CHANGED ");
        printEntry(index);
    }
}

static void updateStat(const char *fileName, const struct stat *statBuf) {
    int entryIndex = findEntry(fileName);  /* find existing entry */
    if (entryIndex == NOT_FOUND)
        entryIndex = addEntry(fileName, statBuf);  /* add new entry */
    else
        updateEntry(entryIndex, statBuf);  /* update existing entry */
    if (entryIndex != NOT_FOUND)
        stats[entryIndex].thisCycle = true;  /* seen in this scan */
}

static void processDirectory(const char *dirName) {
    DIR *dir = opendir(dirName);  /* open for reading */
    if (dir == NULL) {
        perror(dirName);
        return;
    }
    struct dirent *entry;
    char fileName[MAX_FILENAME];
    while ((entry = readdir(dir)) != NULL) {  /* all directory entries */
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;  /* skip . and .. */
        if (snprintf(fileName, sizeof fileName, "%s/%s", dirName,
                     entry->d_name) >= (int)sizeof fileName)
            continue;  /* name too long for the table */
        monitorFile(fileName);  /* call recursively */
    }
    closedir(dir);
}

static void monitorFile(const char *fileName) {  /* a file or directory */
    struct stat statBuf;
    if (stat(fileName, &statBuf) == -1) {  /* obtain file status */
        if (errno != ENOENT)  /* a missing file may appear later */
            perror(fileName);
        return;
    }
    mode_t mode = statBuf.st_mode;  /* mode of file */
    if (S_ISDIR(mode))  /* directory */
        processDirectory(fileName);
    else if (S_ISREG(mode) || S_ISCHR(mode) || S_ISBLK(mode))
        updateStat(fileName, &statBuf);  /* regular or special file */
}

static void monitorFiles(void) {  /* one scan of all files */
    for (int i = 0; i < fileCount; i++)
        monitorFile(fileNames[i]);

    for (int i = 0; i < MAX_FILES; i++) {  /* update the table */
        if (stats[i].lastCycle && !stats[i].thisCycle)
            printf("DELETED %s\n", stats[i].fileName);
        stats[i].lastCycle = stats[i].thisCycle;
        stats[i].thisCycle = false;
    }
}

static void monitorLoop(void) {  /* the main monitor loop */
    do {
        monitorFiles();   /* scan all files */
        fflush(stdout);   /* the output may be a file */
        fflush(stderr);
        sleep(delayTime); /* wait until the next scan */
    } while (loopCount == FOREVER || --loopCount > 0);
}

int main(int argc, char *argv[]) {
    parseCommandLine(argc, argv);  /* parse the command line */
    monitorLoop();                 /* execute the main monitor loop */
    return 0;
}
