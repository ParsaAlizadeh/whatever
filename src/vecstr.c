#include <stdlib.h>
#include <string.h>
#include "vecstr.h"

string *string_new(void) {
    string *this = malloc(sizeof(string));
    this->cap = 4;
    this->size = 0;
    this->seq = malloc(this->cap);
    this->seq[0] = '\0';
    return this;
}

static void string_recap(string *this, int newcap) {
    if (newcap <= this->size+1)
        return;
    char *newseq = malloc(newcap);
    strncpy(newseq, this->seq, this->size+1);
    free(this->seq);
    this->seq = newseq;
    this->cap = newcap;
}

void string_push(string *this, char c) {
    if (this->size+1 == this->cap)
        string_recap(this, this->cap * 2);
    this->seq[this->size] = c;
    this->seq[++this->size] = '\0';
}

char *string_free(string *this) {
    char *seq = this->seq;
    free(this);
    return seq;
}
