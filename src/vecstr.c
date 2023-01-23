#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vecstr.h"
#include "fileutil.h"

string *string_new(void) {
    string *this = malloc(sizeof(string));
    this->seq = NULL;
    this->size = 0;
    this->f = open_memstream(&this->seq, &this->size);
    return this;
}

string *string_using(char **seq) {
    string *this = malloc(sizeof(string));
    this->seq = NULL;
    this->size = 0;
    this->f = open_memstream(seq, &this->size);
    return this;
}

char *string_free(string *this) {
    fclose(this->f);
    char *seq = this->seq;
    free(this);
    return seq;
}

size_t string_size(string *this) {
    fflush(this->f);
    return this->size;
}

char *string_insert(const char *str, int size, int ind, char chr) {
    if (ind < 0 || ind > size)
        return NULL;
    char *res = malloc(size+2);
    for (int i = 0; i < ind; i++)
        res[i] = str[i];
    res[ind] = chr;
    for (int i = ind; i <= size; i++)
        res[i+1] = str[i];
    return res;
}

char *string_erase(const char *str, int size, int ind) {
    if (ind < 0 || ind >= size)
        return NULL;
    char *res = malloc(size);
    for (int i = 0; i < ind; i++)
        res[i] = str[i];
    for (int i = ind+1; i <= size; i++)
        res[i-1] = str[i];
    return res;
}
