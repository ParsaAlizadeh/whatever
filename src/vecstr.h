#ifndef VECSTR_H
#define VECSTR_H

#include <stdio.h>

typedef struct {
    char *seq;
    size_t size;
    FILE *f;
} string;

string *string_new(void);
char *string_free(string *);
size_t string_size(string *);

#endif
