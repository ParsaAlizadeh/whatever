#ifndef VECSTR_H
#define VECSTR_H

struct string {
    char *seq;
    int size, cap;
};

typedef struct string string;

string *string_new(void);
string *string_from(char *);
void string_push(string *, char);
char *string_free(string *);

#endif
