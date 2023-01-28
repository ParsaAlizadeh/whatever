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

void ctx_clear(void) {
    ctx_set(NULL);
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

static const char *ctx_edmode = NULL;

const char *ctx_get_edmode(void) {
    return ctx_edmode;
}

void ctx_set_edmode(const char *edmode) {
    ctx_edmode = edmode;
}

static int ctx_ncmds = 0;
static const command *ctx_allcmds = NULL;

void ctx_set_cmds(int n_cmds, const command *all_cmds) {
    ctx_ncmds = n_cmds;
    ctx_allcmds = all_cmds;
}

int ctx_get_ncmds(void) {
    return ctx_ncmds;
}

const command *ctx_get_allcmds(void) {
    return ctx_allcmds;
}
