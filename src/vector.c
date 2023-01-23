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
    if (this->size+1 == this->cap)
        vector_recap(this, this->cap * 2);
    this->seq[this->size] = item;
    this->seq[++this->size] = NULL;
}

static veccmp_fn_t __cmp;

static int qsort_cmp(const void *a, const void *b) {
    return __cmp(*(void **)a, *(void **)b);
}

void vector_sort(vector *this, veccmp_fn_t cmp) {
    __cmp = cmp;
    qsort(this->seq, this->size, sizeof(void *), qsort_cmp);
}

void vector_free(vector *this) {
    free(this->seq);
    free(this);
}

void vector_freeall(vector *this) {
    for (int i = 0; i < this->size; i++)
        free(this->seq[i]);
    vector_free(this);
}

void vector_insert(vector *this, int ind, void *item) {
    if (ind < 0 || ind > this->size)
        return;
    for (int i = ind; i < this->size; i++) {
        void *nxt = this->seq[i];
        this->seq[i] = item;
        item = nxt;
    }
    vector_push(this, item);
}

void *vector_erase(vector *this, int ind) {
    if (ind < 0 || ind >= this->size)
        return NULL;
    void *item = this->seq[ind];
    for (int i = ind; i < this->size; i++) {
        this->seq[i] = this->seq[i+1];
    }
    this->size--;
    return item;
}
