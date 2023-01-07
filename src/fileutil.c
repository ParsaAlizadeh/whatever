#include "fileutil.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int mkdir_p(const char *_path) {
    if (_path == NULL  || *_path == '\0')
        return 0;
    char *path = strdup(_path);
    char *cur = path[0] == '/' ? path+1 : path;
    char *nxt;
    while ((nxt = strchr(cur, '/')) != NULL) {
        *nxt = '\0';
        if (!fu_isdirectory(path) && mkdir(path, 0777) == -1) {
            free(path);
            return -1;
        }
        *nxt = '/';
        cur = nxt+1;
    }
    if (!fu_isdirectory(path) && mkdir(path, 0777) == -1) {
        free(path);
        return -1;
    }
    free(path);
    return 0;
}

char *fu_dirname(const char *_path) {
    if (_path == NULL  || *_path == '\0')
        return NULL;
    char *path = strdup(_path);
    char *last = strrchr(path, '/');
    if (last == NULL) {
        free(path);
        return strdup(".");
    }
    *last = '\0';
    return path;
}

int fu_exists(const char *path) {
    struct stat buf;
    return stat(path, &buf) == 0;
}

int fu_isdirectory(const char *path) {
    struct stat buf;
    if (stat(path, &buf) == -1)
        return 0;
    return S_ISDIR(buf.st_mode);
}

void fu_copy(FILE *from, FILE *to) {
    int c;
    while ((c = fgetc(from)) != EOF)
        fputc(c, to);
}

int fu_backup(const char *path) {
    FILE *orig, *bak;
    if ((orig = fopen(path, "r")) == NULL)
        return -1;
    int size = strlen(path);
    char *bakpath = malloc(size+2);
    sprintf(bakpath, "%s~", path);
    if ((bak = fopen(bakpath, "w")) == NULL) {
        free(bakpath);
        fclose(orig);
        return -1;
    }
    fu_copy(orig, bak);
    free(bakpath);
    fclose(orig);
    fclose(bak);
    return 0;
}
