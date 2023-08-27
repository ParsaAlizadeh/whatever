#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    void **seq;
    int size, cap;
} vector;

typedef int (*veccmp_fn_t)(const void *, const void *);

extern vector *vector_new(void);
extern void vector_push(vector *, void *);
extern void vector_sort(vector *, veccmp_fn_t);
extern void vector_free(vector *);
extern void vector_freeall(vector *);
extern void vector_insert(vector *, int ind, void *item);
extern void *vector_erase(vector *, int ind);

#endif
