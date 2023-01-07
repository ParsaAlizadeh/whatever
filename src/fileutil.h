#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <stdio.h>
#include "vector.h"

/*
 * returns 0 on success, or -1 on failure and set the errno
 */
int mkdir_p(const char *);

/*
 * returns a malloc(3)ated string representing dirname,
 * or null if path is a null pointer or empty string
 */
char *fu_dirname(const char *);

/*
 * checkers. they return a boolean
 */
int fu_exists(const char *);
int fu_isdirectory(const char *);

void fu_copy(FILE *, FILE *);

/*
 * returns malloc(3)ated string pointing to backup path
 */
char *fu_backuppath(const char *);

/*
 * backup /path/to/file into /path/to/file~
 * returns 0 on success, -1 on failure and set errno
 */
int fu_backup(const char *);

#endif