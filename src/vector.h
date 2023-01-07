#ifndef VECTOR_H
#define VECTOR_H

struct vector {
    void **seq;
    int size, cap;
};

typedef struct vector vector;

vector *vector_new(void);
void vector_push(vector *, void *);
void **vector_free(vector *);

#endif
