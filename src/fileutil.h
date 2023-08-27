#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <stdio.h>
#include "vector.h"
#include "pattern.h"

/*
 * returns 0 on success, or -1 on failure and set the errno
 */
extern int mkdir_p(const char *path);

/*
 * returns a malloc(3)ated string representing dirname and basename,
 * or null if path is a null pointer or empty string
 */
extern char *fu_dirname(const char *path);
extern char *fu_basename(const char *path);

extern int fu_exists(const char *path);
extern int fu_isdirectory(const char *path);

extern FILE *fu_open(const char *path, const char *mode);
extern FILE *fu_tmpfile(void);

extern void fu_copyn(FILE *from, FILE *to, long n);
extern void fu_copy(FILE *from, FILE *to);

extern int fu_backup(const char *path);
extern int fu_restore(const char *path);

extern int fu_fileop_init(const char *path, FILE **file, FILE **tmp);
extern int fu_fileop_end(const char *path, FILE **file, FILE **tmp);

/*
 * returns 0 on success, -1 on failure and set errno
 */
extern int fu_fmodifyat(const char *path, long pos, long n, FILE *from, FILE *to);
extern int fu_finsertat(const char *path, long pos, FILE *from);
extern int fu_insertat(const char *path, long pos, const char *str);
extern int fu_removeat(const char *path, long pos, long n);
extern int fu_pretty(const char *path);

/*
 * find position of file according to line and col
 * return -1 on failure
 */
extern long fu_whereat(FILE *file, long line, long col);
extern long fu_pwhereat(const char *path, long line, long col, long dir, long *n);

/*
 * always assuming pos is a valid position in the file
 */
extern long fu_wordat(FILE *file, long pos);

typedef struct {
    long offset, size;
} subseq_t;

extern subseq_t fu_nextmatch(FILE *file, pattern *pat);

extern char *fu_getline(FILE *file);

#endif