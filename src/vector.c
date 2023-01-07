#include <stdlib.h>
#include <string.h>
#include "vector.h"

vector *vector_new(void) {
    vector *this = malloc(sizeof(vector));
    this->cap = 4;
    this->size = 0;
    this->seq = malloc(sizeof(void *) * this->cap);
    this->seq[0] = NULL;
    return this;
}

static void vector_recap(vector *this, int newcap) {
    if (newcap <= this->size+1)
        return;
    void **newseq = malloc(sizeof(void *) * newcap);
    memcpy(newseq, this->seq, sizeof(void *) * (this->size+1));
    free(this->seq);
    this->seq = newseq;
    this->cap = newcap;
}

void vector_push(vector *this, void *item) {
    if (this->size == this->cap)
        vector_recap(this, this->cap * 2);
    this->seq[this->size] = item;
    this->seq[++this->size] = NULL;
}

void **vector_free(vector *this) {
    void **seq = this->seq;
    free(this);
    return seq;
}
