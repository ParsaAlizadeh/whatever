#include "context.h"

#include <stdlib.h>
#include <string.h>
#include "editor.h"
#include "fileutil.h"
#include "logging.h"

static const char *ctx_file = NULL;
static int buf_mode = 0;

const char *ctx_get(void) {
    if (ctx_get_buf_mode())
        return BUFFER_PATH;
    return ctx_file;
}

void ctx_set(const char *path) {
    if (ctx_file != NULL) {
        ctx_save();
        free((void *)ctx_file);
    }
    if (path != NULL)
        path = strdup(path);
    ctx_file = path;
}

void ctx_save(void) {
    if (ed == NULL)
        return;
    if (!ctx_get_buf_mode() && !ed->modified)
        return;
    const char *ctx = ctx_get();
    fu_backup(ctx);
    if (editor_saveas(ctx) != 0)
        loginfo("ctx: save failed");
    if (!ctx_get_buf_mode())
        ed->modified = 0;
}

int ctx_get_buf_mode(void) {
    return ctx_file == NULL || buf_mode;
}

void ctx_set_buf_mode(int mode) {
    buf_mode = mode;
}

const char *ctx_edmode = NULL;
int ctx_ncmds = 0;
const command *ctx_allcmds = NULL;
