#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <stdio.h>
#include "vector.h"
#include "pattern.h"

/*
 * returns 0 on success, or -1 on failure and set the errno
 */
int mkdir_p(const char *path);

/*
 * returns a malloc(3)ated string representing dirname and basename,
 * or null if path is a null pointer or empty string
 */
char *fu_dirname(const char *path);
char *fu_basename(const char *path);

int fu_exists(const char *path);
int fu_isdirectory(const char *path);

FILE *fu_open(const char *path, const char *mode);

void fu_copyn(FILE *from, FILE *to, long n);
void fu_copy(FILE *from, FILE *to);

int fu_backup(const char *path);
int fu_restore(const char *path);

/*
 * returns 0 on success, -1 on failure and set errno
 */
int fu_fmodifyat(const char *path, long pos, long n, FILE *from, FILE *to);
int fu_finsertat(const char *path, long pos, FILE *from);
int fu_insertat(const char *path, long pos, const char *str);
int fu_removeat(const char *path, long pos, long n);

/*
 * find position of file according to line and col
 * return -1 on failure
 */
long fu_whereat(FILE *file, long line, long col);
long fu_pwhereat(const char *path, long line, long col, long dir, long *n);

/*
 * always assuming pos is a valid position in the file
 */
long fu_wordat(FILE *file, long pos);

typedef struct {
    long offset, size;
} subseq_t;

subseq_t fu_nextmatch(FILE *file, pattern *pat);

char *fu_getline(FILE *file);

#endif