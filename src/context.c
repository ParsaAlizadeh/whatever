#include "context.h"

#include <stdlib.h>
#include <string.h>

static const char *ctx_file = NULL;

const char *ctx_get(void) {
    return ctx_file;
}

void ctx_set(const char *path) {
    if (ctx_file != NULL)
        free(ctx_file);
    if (path != NULL)
        path = strdup(path);
    ctx_file = path;
}

void ctx_clear(void) {
    ctx_set(NULL);
}
