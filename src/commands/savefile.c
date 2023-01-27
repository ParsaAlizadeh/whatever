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
    (void)inp;
    (void)out;
    savefile_t *this = _this;
    int is_saveas = (this->path != NULL && strcmp(this->path, BUFFER_PATH) != 0);
    if (is_saveas) {
        ctx_set(this->path);
        ed->modified = 1;
    }
    ctx_set_buf_mode(0);
    if (ctx_get_buf_mode()) {
        cmdlog(&savefile, "saving buffer has no effect");
    } else {
        ctx_save();
    }
}

const command savefile = {
    .name       = "save",
    .optstring  = "f:",
    .usage      = "-f FILE",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};
