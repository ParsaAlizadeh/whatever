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
 * returns a malloc(3)ated string representing dirname,
 * or null if path is a null pointer or empty string
 */
char *fu_dirname(const char *path);

int fu_exists(const char *path);
int fu_isdirectory(const char *path);

void fu_copyn(FILE *from, FILE *to, long n);
void fu_copy(FILE *from, FILE *to);

/*
 * returns malloc(3)ated string pointing to backup path
 */
char *fu_backuppath(const char *path);

/*
 * backup /path/to/file into /path/to/file~
 * returns 0 on success, -1 on failure and set errno
 */
int fu_backup(const char *path);

int fu_insertat(const char *path, long pos, const char *str);
int fu_removeat(const char *path, long pos, long n);

char *fu_readat(FILE *file, long pos, long n);
char *fu_preadat(const char *path, long pos, long n);

long fu_whereat(FILE *file, long line, long col);
long fu_pwhereat(const char *path, long line, long col, long dir, long *n);

long fu_wordat(FILE *file, long pos);
long fu_extendleft(FILE *file, long pos);

pattern *fu_nextmatch(FILE *file, pattern *pat);

#endif