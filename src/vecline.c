#include "vecline.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileutil.h"
#include "vecstr.h"

line_t *line_new(char *content, int size) {
    line_t *this = malloc(sizeof(line_t));
    this->content = content;
    this->size = size;
    return this;
}

void line_free(line_t *this) {
    free(this->content);
    free(this);
}

int line_insert(line_t *this, int col, char chr) {
    char *newc = string_insert(this->content, this->size, col, chr);
    if (newc == NULL)
        return 0;
    free(this->content);
    this->content = newc;
    this->size++;
    return 1;
}

int line_erase(line_t *this, int col) {
    char *newc = string_erase(this->content, this->size, col);
    if (newc == NULL)
        return 0;
    free(this->content);
    this->content = newc;
    this->size--;
    return 1;
}

void line_append(line_t *this, line_t *oth) {
    char *newc = malloc(this->size + oth->size + 1);
    strncpy(newc, this->content, this->size+1);
    strncat(newc, oth->content, oth->size);
    free(this->content);
    this->content = newc;
    this->size += oth->size;
}

line_t *line_partition(line_t *this, int col) {
    if (col < 0 || col > this->size)
        return NULL;
    int size = this->size;
    char *lc = malloc(col+1);
    char *rc = malloc(size-col+1);
    memcpy(lc, this->content, col);
    lc[col] = '\0';
    memcpy(rc, this->content+col, size-col);
    rc[size-col] = '\0';
    free(this->content);
    this->content = lc;
    this->size = col;
    return line_new(rc, size-col);
}

vecline *vc_new0(void) {
    vecline *this = malloc(sizeof(vecline));
    this->lines = vector_new();
    return this;
}

vecline *vc_new1(void) {
    vecline *this = vc_new0();
    vector_push(this->lines, line_new(strdup(""), 0));
    return this;
}

vecline *vc_newfile(FILE *file) {
    vecline *this = vc_new0();
    char *sline;
    while ((sline = fu_getline(file)) != NULL)
        vector_push(this->lines, line_new(sline, strlen(sline)));
    return this;
}

vecline *vc_newpath(const char *path) {
    FILE *file = fu_open(path, "r");
    if (file == NULL)
        return NULL;
    vecline *this = vc_newfile(file);
    fclose(file);
    return this;
}

vecline *vc_newstr(const char *str) {
    FILE *file = fmemopen((char *)str, strlen(str), "r");
    vecline *this = vc_newfile(file);
    fclose(file);
    return this;
}

void vc_writefile(vecline *this, FILE *file) {
    for (int i = 0; i < vc_nlines(this); i++) {
        line_t *line = vc_atline(this, i);
        FILE *fline = fmemopen(line->content, line->size, "r");
        if (i > 0)
            fprintf(file, "\n");
        fu_copy(fline, file);
        fclose(fline);
    }
}

int vc_writepath(vecline *this, const char *path) {
    FILE *file = fu_open(path, "w");
    if (file == NULL)
        return -1;
    vc_writefile(this, file);
    fclose(file);
    return 0;
}

int vc_nlines(vecline *this) {
    return this->lines->size;
}

line_t *vc_atline(vecline *this, int row) {
    if (row < 0 || row >= vc_nlines(this))
        return NULL;
    return this->lines->seq[row];
}

void vc_free(vecline *this) {
    for (int row = 0; row < vc_nlines(this); row++)
        line_free(vc_atline(this, row));
    vector_free(this->lines);
    free(this);
}

int vc_at(vecline *this, pos_t apos) {
    line_t *line = vc_atline(this, apos.line);
    if (line == NULL || apos.col > line->size)
        return -1;
    return line->content[apos.col];
}

pos_t vc_insert(vecline *this, pos_t apos, char chr) {
    line_t *line = vc_atline(this, apos.line);
    if (line == NULL)
        return apos;
    if (apos.col < 0 || apos.col > line->size)
        return apos;
    if (chr == '\n') {
        line_t *newline = line_partition(line, apos.col);
        vector_insert(this->lines, apos.line+1, newline);
        return (pos_t) { apos.line+1, 0 };
    }
    if (line_insert(line, apos.col, chr))
        return (pos_t) { apos.line, apos.col+1 };
    return apos;
}

pos_t vc_erase(vecline *this, pos_t apos) {
    line_t *line = vc_atline(this, apos.line);
    if (line == NULL)
        return apos;
    if (apos.col < 0 || apos.col > line->size)
        return apos;
    if (apos.col == 0) {
        line_t *prvline = vc_atline(this, apos.line-1);
        if (prvline == NULL)
            return apos;
        int prvsize = prvline->size;
        line_append(prvline, line);
        line_free(line);
        vector_erase(this->lines, apos.line);
        return (pos_t) { apos.line-1, prvsize };
    }
    if (line_erase(line, apos.col-1))
        return (pos_t) { apos.line, apos.col-1 };
    return apos;
}

void vc_log(vecline *this) {
    int nlines = vc_nlines(this);
    printf("nlines=%d\n", nlines);
    for (int i = 0; i < nlines; i++) {
        printf("%d: %s\n", i, vc_atline(this, i)->content);
    }
}
