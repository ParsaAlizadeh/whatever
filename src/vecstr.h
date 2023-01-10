#ifndef VECSTR_H
#define VECSTR_H

typedef struct {
    char *seq;
    int size, cap;
} string;

string *string_new(void);
string *string_from(char *);
char *string_free(string *);
void string_push(string *, char);
void string_pushes(string *, const char *);
void string_clear(string *);

#endif
