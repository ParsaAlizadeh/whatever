#include "context.h"

#include <stdlib.h>
#include <string.h>
#include "editor.h"
#include "fileutil.h"
#include "logging.h"

ctx_mode_t ctx_mode = CTX_NULL;
int ctx_counter = 0;
static const char *ctx_file = NULL;

const char *ctx_get(void) {
    switch (ctx_mode) {
    case CTX_NULL:
        return NULL;
    case CTX_BUFFER:
        return BUFFER_PATH;
    case CTX_PATH:
        return ctx_file;
    }
    return NULL;
}

void ctx_set(ctx_mode_t mode, const char *path) {
    if (ctx_file != NULL) {
        free((void *)ctx_file);
    }
    if (path != NULL)
        path = strdup(path);
    ctx_mode = mode;
    ctx_file = path;
}

void ctx_fix(void) {
    if (ctx_file == NULL)
        ctx_mode = CTX_NULL;
    else
        ctx_mode = CTX_PATH;
}

const char *ctx_edmode = NULL;
int ctx_ncmds = 0;
const command *ctx_allcmds = NULL;
