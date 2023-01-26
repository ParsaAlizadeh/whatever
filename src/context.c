#include "context.h"

#include <stdlib.h>
#include <string.h>
#include "editor.h"

static const char *ctx_file = NULL;
static int buf_mode = 0;

const char *ctx_get(void) {
    if (ctx_get_buf_mode())
        return BUFFER_PATH;
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

void ctx_save(void) {
    if (ed == NULL)
        return;
    const char *ctx = ctx_get();
    editor_saveas(ctx);
}

int ctx_get_buf_mode(void) {
    return ctx_file == NULL || buf_mode;
}

void ctx_set_buf_mode(int mode) {
    buf_mode = mode;
}

static const char *ctx_edmode = NULL;

const char *ctx_get_edmode(void) {
    return ctx_edmode;
}

void ctx_set_edmode(const char *edmode) {
    ctx_edmode = edmode;
}
