#include "fileutil.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <ctype.h>
#include "vecstr.h"
#include "context.h"
#include "parse.h"
#include "settings.h"

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

char *fu_basename(const char *path) {
    if (path == NULL || *path == '\0')
        return NULL;
    const char *last = strrchr(path, '/');
    if (last == NULL)
        return strdup(path);
    return strdup(last+1);
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

static int _fu_is_read(const char *mode) {
    return strcmp(mode, "r") == 0 || strcmp(mode, "rb") == 0;
}

static void _fu_update_counter(const char *path, const char *mode) {
    const char *ctx = ctx_get();
    if (ctx && !_fu_is_read(mode) && strcmp(path, ctx) == 0)
        ctx_counter++;
}

FILE *fu_open(const char *path, const char *mode) {
    if (fu_isdirectory(path)) {
        errno = EISDIR;
        return NULL;
    }
    if (IS_IDIOT) {
        char *dir = fu_dirname(path);
        if (!fu_isdirectory(dir)) {
            /* I am aware EFAULT is a socket error, hence the IS_IDIOT */
            errno = EFAULT;
            free(dir);
            return NULL;
        }
        free(dir);
    }
    _fu_update_counter(path, mode);
    return fopen(path, mode);
}

FILE *fu_tmpfile(void) {
    FILE *file = tmpfile();
    if (file == NULL)
        err(EXIT_FAILURE, "tmpfile");
    return file;
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

static char *fu_backuppath(const char *path, int n) {
    if (n == 0)
        return strdup(path);
    char *dir = fu_dirname(path);
    char *base = fu_basename(path);
    string *bakpath = string_new();
    fprintf(bakpath->f, "%s/.%s~%d", dir, base, n);
    free(dir);
    free(base);
    return string_free(bakpath);
}

static void fu_push_backups(const char *path, const char *cur, int n) {
    if (!fu_exists(cur))
        return;
    char *nxt = fu_backuppath(path, n+1);
    fu_push_backups(path, nxt, n+1);
    rename(cur, nxt);
    free(nxt);
}

static int fu_pop_backups(const char *path, const char *cur, int n) {
    char *nxt = fu_backuppath(path, n+1);
    int exists = fu_exists(nxt);
    if (exists) {
        rename(nxt, cur);
        fu_pop_backups(path, nxt, n+1);
    }
    free(nxt);
    return n == 0 && exists;
}

int fu_backup(const char *path) {
    if (strcmp(path, BUFFER_PATH) == 0)
        return 0;
    FILE *orig, *bak;
    if ((orig = fu_open(path, "r")) == NULL)
        return -1;
    char *bakpath = fu_backuppath(path, 1);
    fu_push_backups(path, bakpath, 1);
    if ((bak = fu_open(bakpath, "w")) == NULL) {
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

int fu_restore(const char *path) {
    if (strcmp(path, BUFFER_PATH) == 0)
        return 0;
    if (fu_pop_backups(path, path, 0))
        return 0;
    return -1;
}

int fu_fileop_init(const char *path, FILE **file, FILE **tmp) {
    if ((*file = fu_open(path, "r")) == NULL)
        return -1;
    *tmp = fu_tmpfile();
    return 0;
}

int fu_fileop_end(const char *path, FILE **file, FILE **tmp) {
    fclose(*file);
    if ((*file = fu_open(path, "w")) == NULL) {
        fclose(*tmp);
        return -1;
    }
    rewind(*tmp);
    fu_copy(*tmp, *file);
    fclose(*file);
    fclose(*tmp);
    return 0;
}

int fu_fmodifyat(const char *path, long pos, long n, FILE *from, FILE *to) {
    FILE *file, *tmp;
    if (fu_fileop_init(path, &file, &tmp) != 0)
        return -1;
    fu_copyn(file, tmp, pos);
    fu_copyn(file, to, n);
    if (from != NULL)
        fu_copy(from, tmp);
    fu_copy(file, tmp);
    if (fu_fileop_end(path, &file, &tmp) != 0)
        return -1;
    return 0;
}

int fu_finsertat(const char *path, long pos, FILE *from) {
    return fu_fmodifyat(path, pos, 0, from, NULL);
}

int fu_insertat(const char *path, long pos, const char *str) {
    FILE *from = fmemopen((char *)str, strlen(str), "r");
    int rc = fu_finsertat(path, pos, from);
    fclose(from);
    return rc;
}

int fu_removeat(const char *path, long pos, long n) {
    return fu_fmodifyat(path, pos, n, NULL, NULL);
}

int fu_pretty(const char *path) {
    FILE *file, *tmp;
    if (fu_fileop_init(path, &file, &tmp) != 0)
        return -1;
    prettify(file, tmp);
    if (fu_fileop_end(path, &file, &tmp) != 0)
        return -1;
    return 0;
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
    if ((file = fu_open(path, "r")) == NULL)
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

static subseq_t fu_subseqmatched(pattern *pat) {
    subseq_t ss;
    ss.offset = pattern_matched(pat);
    if (ss.offset == -1)
        return ss;
    ss.size = pat->visited - ss.offset;
    return ss;
}

subseq_t fu_nextmatch(FILE *file, pattern *pat) {
    int chr;
    subseq_t sprv, snxt;
    sprv.offset = -1;
    while ((chr = fgetc(file)) != EOF) {
        pattern_feed(pat, chr, 1);
        snxt = fu_subseqmatched(pat);
        if (snxt.offset == -1 && sprv.offset != -1)
            return sprv;
        sprv = snxt;
    }
    return sprv;
}

char *fu_getline(FILE *file) {
    if (feof(file))
        return NULL;
    string *line = string_new();
    int chr;
    while ((chr = fgetc(file)) != EOF) {
        if (chr == '\n')
            break;
        fprintf(line->f, "%c", (char)chr);
    }
    return string_free(line);
}
