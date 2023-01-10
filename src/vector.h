#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    void **seq;
    int size, cap;
} vector;

typedef int (*veccmp_fn_t)(const void *, const void *);

vector *vector_new(void);
void vector_push(vector *, void *);
void vector_sort(vector *, veccmp_fn_t);
void vector_free(vector *);
void vector_freeall(vector *);

#endif
