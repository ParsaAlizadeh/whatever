#ifndef VECSTR_H
#define VECSTR_H

#include <stdio.h>

typedef struct {
    char *seq;
    size_t size;
    FILE *f;
} string;

extern string *string_new(void);
extern string *string_using(char **);
extern char *string_free(string *);
extern size_t string_size(string *);

extern char *string_insert(const char *str, int size, int ind, char chr);
extern char *string_erase(const char *str, int size, int ind);

#endif
