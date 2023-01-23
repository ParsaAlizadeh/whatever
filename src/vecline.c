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

void line_insert(line_t *this, int col, char chr) {
    char *newc = string_insert(this->content, this->size, col, chr);
    if (newc == NULL)
        return;
    free(this->content);
    this->content = newc;
    this->size++;
}

void line_erase(line_t *this, int col) {
    char *newc = string_erase(this->content, this->size, col);
    if (newc == NULL)
        return;
    free(this->content);
    this->content = newc;
    this->size--;
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

vecline *vc_new(void) {
    vecline *this = malloc(sizeof(vecline));
    this->lines = vector_new();
    return this;
}

vecline *vc_newfile(FILE *file) {
    vecline *this = vc_new();
    string *strline = string_new();
    int chr;
    while (1) {
        chr = fgetc(file);
        if (chr == '\n' || chr == EOF) {
            int size = string_size(strline);
            char *content = string_free(strline);
            vector_push(this->lines, line_new(content, size));
            if (chr != EOF)
                strline = string_new();
        }
        if (chr == EOF)
            break;
        if (chr != '\n')
            fprintf(strline->f, "%c", chr);
    }
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

int vc_nlines(vecline *this) {
    return this->lines->size;
}

line_t *vc_atline(vecline *this, int row) {
    if (row >= vc_nlines(this))
        return NULL;
    return this->lines->seq[row];
}

void vc_free(vecline *this) {
    for (int row = 0; row < vc_nlines(this); row++)
        line_free(vc_atline(this, row));
    vector_free(this->lines);
    free(this);
}

int vc_at(vecline *this, int row, int col) {
    line_t *line = vc_atline(this, row);
    if (line == NULL || col > line->size)
        return -1;
    return line->content[col];
}

void vc_insert(vecline *this, int row, int col, char chr) {
    line_t *line = vc_atline(this, row);
    if (line == NULL)
        return;
    if (col < 0 || col > line->size)
        return;
    if (chr == '\n') {
        line_t *newline = line_partition(line, col);
        vector_insert(this->lines, row+1, newline);
        return;
    }
    line_insert(line, col, chr);
}

void vc_erase(vecline *this, int row, int col) {
    line_t *line = vc_atline(this, row);
    if (line == NULL)
        return;
    if (col < 0 || col > line->size)
        return;
    if (col == 0) {
        line_t *prvline = vc_atline(this, row-1);
        if (prvline == NULL)
            return;
        line_append(prvline, line);
        line_free(line);
        vector_erase(this->lines, row);
        return;
    }
    line_erase(line, col-1);
}

void vc_log(vecline *this) {
    int nlines = vc_nlines(this);
    printf("nlines=%d\n", nlines);
    for (int i = 0; i < nlines; i++) {
        printf("%d: %s\n", i, vc_atline(this, i)->content);
    }
}
