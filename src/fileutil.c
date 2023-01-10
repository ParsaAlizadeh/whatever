#include "fileutil.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
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

void fu_copyn(FILE *from, FILE *to, long n) {
    long ind;
    int chr;
    for (ind = 0; (n == -1 || ind < n) && (chr = fgetc(from)) != EOF; ind++)
        if (to != NULL)
            fputc(chr, to);
}

void fu_copy(FILE *from, FILE *to) {
    return fu_copyn(from, to, -1);
}

char *fu_backuppath(const char *path) {
    char *bakpath = malloc(strlen(path)+2);
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

int fu_insertat(const char *path, long pos, const char *str) {
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
    rewind(tmp);
    fu_copy(tmp, file);
    fclose(file);
    fclose(tmp);
    return 0;
}

int fu_removeat(const char *path, long pos, long n) {
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
    rewind(tmp);
    fu_copy(tmp, file);
    fclose(file);
    fclose(tmp);
    return 0;
}

char *fu_readat(FILE *file, long pos, long n) {
    long before = ftell(file);
    fseek(file, pos, SEEK_SET);
    string *str = string_new();
    long ind;
    int chr;
    for (ind = 0; ind < n && (chr = fgetc(file)) != EOF; ind++)
        string_push(str, chr);
    fseek(file, before, SEEK_SET);
    return string_free(str);
}

char *fu_preadat(const char *path, long pos, long n) {
    FILE *file;
    if ((file = fopen(path, "r")) == NULL)
        return NULL;
    char *content = fu_readat(file, pos, n);
    fclose(file);
    return content;
}

long fu_whereat(FILE *file, long line, long col) {
    long before = ftell(file);
    long cur_l = 1, cur_c = 0, ind = 0;
    int chr = EOF;
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
    if (cur_l != line || cur_c != col)
        return -1;
    return before + ind;
}

long fu_pwhereat(const char *path, long line, long col, long dir, long *n) {
    FILE *file;
    if ((file = fopen(path, "r")) == NULL)
        return -1;
    long pos = fu_whereat(file, line, col);
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

long fu_wordat(FILE *file, long pos) {
    long before = ftell(file);
    rewind(file);
    long cnt = 1;
    int lastnotspace = 0;
    int chr;
    long ind;
    for (ind = 0; ind <= pos && (chr = fgetc(file)) != EOF; ind++) {
        if (isspace(chr)) {
            cnt += lastnotspace;
            lastnotspace = 0;
        } else {
            lastnotspace = 1;
        }
    }
    fseek(file, before, SEEK_SET);
    return cnt;
}

long fu_extendleft(FILE *file, long pos) {
    long before = ftell(file);
    rewind(file);
    long start = 0;
    int lastspace = 0;
    int chr;
    long ind;
    for (ind = 0; ind <= pos && (chr = fgetc(file)) != EOF; ind++) {
        if (isspace(chr)) {
            start = ind+1;
            lastspace = 1;
        } else {
            lastspace = 0;
        }
    }
    start -= lastspace;
    fseek(file, before, SEEK_SET);
    return start;
}

pattern *fu_nextmatch(FILE *file, pattern *pat) {
    int chr;
    while ((chr = fgetc(file)) != EOF) {
        pattern_feed(pat, chr);
        if (pattern_matched(pat))
            return pat;
    }
    return NULL;
}

subseq_t fu_extend(FILE *file, pattern *pat) {
    subseq_t ss;
    long pat_start = pattern_start(pat);
    ss.offset = pat_start;
    ss.size = pat->size;
    if (pat->wildprefix) {
        ss.offset = fu_extendleft(file, pat_start);
        ss.size += pat_start - ss.offset;
    }
    if (pat->wildsuffix) {
        long before = ftell(file);
        rewind(file);
        fu_copyn(file, NULL, ss.offset + ss.size);
        int chr;
        while ((chr = fgetc(file)) != EOF) {
            if (isspace(chr))
                break;
            ss.size++;
        }
        fseek(file, before, SEEK_SET);
    }
    return ss;
}
