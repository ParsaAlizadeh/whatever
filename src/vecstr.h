#ifndef VECSTR_H
#define VECSTR_H

struct string {
    char *seq;
    int size, cap;
};

typedef struct string string;

string *string_new(void);
string *string_from(char *);
char *string_free(string *);
void string_push(string *, char);
void string_clear(string *);

#endif
