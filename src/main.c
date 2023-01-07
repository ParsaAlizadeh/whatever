#include <stdio.h>
#include <stdlib.h>
#include "parse.h"

int main() {
    vector *vec = scan_line();
    printf("vec size=%d cap=%d\n", vec->size, vec->cap);
    for (int i = 0; i < vec->size; i++) {
        char *tk = vec->seq[i];
        printf("======\n%s\n======\n", tk);
    }
    vector_freeall(vec);
    return EXIT_SUCCESS;
}