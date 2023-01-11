#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vecstr.h"

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
