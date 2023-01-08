#include "fileutil.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "vecstr.h"

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

void fu_copyn(FILE *from, FILE *to, int n) {
    int ind, chr;
    for (ind = 0; (n == -1 || ind < n) && (chr = fgetc(from)) != EOF; ind++)
        if (to != NULL)
            fputc(chr, to);
}

void fu_copy(FILE *from, FILE *to) {
    return fu_copyn(from, to, -1);
}

char *fu_backuppath(const char *path) {
    int size = strlen(path);
    char *bakpath = malloc(size+2);
    sprintf(bakpath, "%s~", path);
    return bakpath;
}

int fu_backup(const char *path) {
    FILE *orig, *bak;
    if ((orig = fopen(path, "r")) == NULL)
        return -1;
    char *bakpath = fu_backuppath(path);
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

int fu_insertat(const char *path, int pos, const char *str) {
    FILE *file, *tmp;
    if ((file = fopen(path, "r")) == NULL)
        return -1;
    if ((tmp = tmpfile()) == NULL) {
        fclose(file);
        return -1;
    }
    fu_copyn(file, tmp, pos);
    fputs(str, tmp);
    fu_copy(file, tmp);
    fflush(tmp);
    fclose(file);
    if ((file = fopen(path, "w")) == NULL) {
        fclose(tmp);
        return -1;
    }
    fseek(tmp, 0, SEEK_SET);
    fu_copy(tmp, file);
    fclose(file);
    fclose(tmp);
    return 0;
}

int fu_removeat(const char *path, int pos, int n) {
    FILE *file, *tmp;
    if ((file = fopen(path, "r")) == NULL)
        return -1;
    if ((tmp = tmpfile()) == NULL) {
        fclose(file);
        return -1;
    }
    fu_copyn(file, tmp, pos);
    fu_copyn(file, NULL, n);
    fu_copy(file, tmp);
    fflush(tmp);
    fclose(file);
    if ((file = fopen(path, "w")) == NULL) {
        fclose(tmp);
        return -1;
    }
    fseek(tmp, 0, SEEK_SET);
    fu_copy(tmp, file);
    fclose(file);
    fclose(tmp);
    return 0;
}

char *fu_readat(FILE *file, int pos, int n) {
    int before = ftell(file);
    fseek(file, pos, SEEK_SET);
    string *str = string_new();
    int ind, chr;
    for (ind = 0; ind < n && (chr = fgetc(file)) != EOF; ind++)
        string_push(str, chr);
    fseek(file, before, SEEK_SET);
    return string_free(str);
}

char *fu_preadat(const char *path, int pos, int n) {
    FILE *file;
    if ((file = fopen(path, "r")) == NULL)
        return NULL;
    char *content = fu_readat(file, pos, n);
    fclose(file);
    return content;
}

int fu_whereat(FILE *file, int line, int col) {
    int before = ftell(file);
    int cur_l = 1, cur_c = 0, chr = EOF, ind = 0;
    do {
        if (chr != EOF)
            cur_c++;
        if (chr == '\n')
            cur_l++, cur_c = 0;
        if (cur_l == line && cur_c == col)
            break;
        ind++;
    } while ((chr = fgetc(file)) != EOF);
    fseek(file, before, SEEK_SET);
    if (cur_l != line && cur_c != col)
        return -1;
    return before + ind;
}

int fu_pwhereat(const char *path, int line, int col, int dir, int *n) {
    FILE *file;
    if ((file = fopen(path, "r")) == NULL)
        return -1;
    int pos = fu_whereat(file, line, col);
    fclose(file);
    if (pos == -1)
        return -1;
    if (dir == -1)
        pos -= *n - 1;
    if (pos < 0) {
        *n += pos;
        pos = 0;
    }
    return pos;
}
