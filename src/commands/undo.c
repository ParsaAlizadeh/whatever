#include "undo.h"
#include "setup.h"

typedef struct {
    char *path;
} undo_t;

static void *make(void) {
    undo_t *this = malloc(sizeof(undo_t));
    this->path = 0;
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
    char *bakpath = fu_backuppath(this->path);
    if (!fu_exists(bakpath))
        return (void)cmdlog(&undo, "no backup exists");
    if (rename(bakpath, this->path) == -1)
        return (void)cmdlog(&undo, "rename failed: %s",
            strerror(errno));
    cmdlog(&undo, "done");
}

const command undo = {
    .name       = "undo",
    .optstring  = ":f:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};