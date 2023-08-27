#include "undo.h"
#include "setup.h"

typedef struct {
    char *path;
} undo_t;

static void *make(void) {
    undo_t *this = malloc(sizeof(undo_t));
    this->path = NULL;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    undo_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('f', path)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this, char *inp, char **out) {
    (void)inp;
    (void)out;
    undo_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&undo, 'f');
    if (ed != NULL && strcmp(this->path, BUFFER_PATH) == 0) {
        editor_run_end(NULL);
        if (ctx_get() == NULL)
            return (void)cmdlog(&undo, "no file has been set");
        if (!ed->modified && fu_restore(ctx_get()) == -1)
            return (void)cmdlog(&undo, "no backup exists");
        editor_readctx();
        editor_reset();
        editor_run_init();
        return;
    }
    if (fu_restore(this->path) == -1)
        return (void)cmdlog(&undo, "no backup exists");
}

const command undo = {
    .name       = "undo",
    .optstring  = "f:",
    .usage      = "[-f FILE]",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};
