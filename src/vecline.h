#ifndef VECLINE_H
#define VECLINE_H

#include <stdio.h>
#include "vector.h"

typedef struct {
    char *content;
    int size;
} line_t;

typedef struct {
    vector *lines;
} vecline;

line_t *line_new(char *content, int size);
void line_free(line_t *this);

void line_insert(line_t *this, int col, char chr);
void line_erase(line_t *this, int col);

void line_append(line_t *this, line_t *oth);
line_t *line_partition(line_t *this, int col);

vecline *vc_new0(void);
vecline *vc_new1(void);
vecline *vc_newfile(FILE *file);
vecline *vc_newpath(const char *path);

int vc_nlines(vecline *this);
line_t *vc_atline(vecline *this, int row);
void vc_free(vecline *this);
int vc_at(vecline *this, int row, int col);

void vc_insert(vecline *this, int row, int col, char chr);
void vc_erase(vecline *this, int row, int col);

void vc_log(vecline *this);

#endif