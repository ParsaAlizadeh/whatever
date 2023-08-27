#ifndef VECLINE_H
#define VECLINE_H

#include <stdio.h>
#include "vector.h"
#include "position.h"

typedef struct {
    char *content;
    int size;
} line_t;

typedef struct {
    vector *lines;
} vecline;

extern line_t *line_new(char *content, int size);
extern void line_free(line_t *);

extern int line_insert(line_t *, int col, char chr);
extern int line_erase(line_t *, int col);

extern void line_append(line_t *, line_t *oth);
extern line_t *line_partition(line_t *, int col);

extern vecline *vc_new0(void);
extern vecline *vc_new1(void);
extern vecline *vc_newfile(FILE *file);
extern vecline *vc_newpath(const char *path);
extern vecline *vc_newstr(const char *str);

extern void vc_writefile(vecline *, FILE *file);
extern int vc_writepath(vecline *, const char *path);

extern int vc_nlines(vecline *);
extern line_t *vc_atline(vecline *, int row);
extern void vc_free(vecline *);
extern int vc_at(vecline *, pos_t apos);

extern pos_t vc_fromtpos(vecline *, int tpos);
extern int vc_totpos(vecline *, pos_t apos);

extern pos_t vc_insert(vecline *, pos_t apos, char chr);
extern pos_t vc_erase(vecline *, pos_t apos);

extern void vc_log(vecline *);

#endif