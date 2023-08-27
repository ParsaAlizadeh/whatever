#include "savefile.h"
#include "setup.h"

typedef struct {
    char *path;
} savefile_t;

static void *make(void) {
    savefile_t *this = malloc(sizeof(savefile_t));
    this->path = NULL;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    savefile_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('f', path)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this, char *inp, char **out) {
    (void)out;
    savefile_t *this = _this;
    if (ed == NULL)
        return (void)cmdlog(&savefile, "nothing to save");
    editor_run_end(inp);
    int is_saveas = (strcmp(this->path, BUFFER_PATH) != 0);
    if (is_saveas)
        ctx_set(CTX_PATH, this->path);
    const char *ctx = ctx_get();
    if (ctx != NULL) {
        editor_writectx();
        editor_reset();
    } else {
        cmdlog(&savefile, "saving buffer has no effect");
    }
    editor_run_init();
}

const command savefile = {
    .name       = "save",
    .optstring  = "f:",
    .usage      = "[-f FILE]",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};
