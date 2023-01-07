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

static int set_opt(void *_this, int c, char *arg) {
    undo_t *this = _this;
    switch (c) {
    case 'f':
        if (this->path != NULL)
            return CMD_REPEATED_OPTION;
        this->path = arg;
        break;
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this) {
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
    .run        = run
};