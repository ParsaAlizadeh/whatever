#include "undo.h"
#include "setup.h"

typedef struct {
    char *path, *bakpath;
} undo_t;

static void *make(void) {
    undo_t *this = malloc(sizeof(undo_t));
    this->path = this->bakpath = NULL;
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
    this->bakpath = fu_backuppath(this->path);
    if (!fu_exists(this->bakpath))
        return (void)cmdlog(&undo, "no backup exists");
    if (rename(this->bakpath, this->path) == -1)
        return (void)cmdlog(&undo, "rename failed: %s",
            strerror(errno));
    cmdlog(&undo, "done");
}

static void undo_free(void *_this) {
    undo_t *this = _this;
    if (this->bakpath != NULL)
        free(this->bakpath);
    free(this);
}

const command undo = {
    .name       = "undo",
    .optstring  = ":f:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = undo_free
};
