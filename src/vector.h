#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    void **seq;
    int size, cap;
} vector;

vector *vector_new(void);
void vector_push(vector *, void *);
void **vector_free(vector *);
void vector_freeall(vector *);

#endif
