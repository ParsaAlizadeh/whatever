#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <stdio.h>
#include "vector.h"

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

void fu_copyn(FILE *from, FILE *to, int n);
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

int fu_insertat(const char *path, int pos, const char *str);
int fu_removeat(const char *path, int pos, int n);

char *fu_readat(FILE *file, int pos, int n);
char *fu_preadat(const char *path, int pos, int n);

int fu_whereat(FILE *file, int line, int col);
int fu_pwhereat(const char *path, int line, int col, int dir, int *n);

#endif