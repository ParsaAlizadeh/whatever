#include "removestr.h"
#include "setup.h"

typedef struct {
    char *path;
    int line_no, col_no;
    int n;
    int direction;
} removestr_t;

static void *make(void) {
    removestr_t *this = malloc(sizeof(removestr_t));
    this->path = NULL;
    this->line_no = this->col_no = this->n = -1;
    this->direction = 0;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    removestr_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('f', path)
    SINGLE_OPTION_POSITION('p', removestr, line_no, col_no)
    SINGLE_OPTION_SCANF('n', removestr, n, -1, "%u")
    SINGLE_OPTION_CONSTANT('>', direction, 0, 1)
    SINGLE_OPTION_CONSTANT('<', direction, 0, -1)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this) {
    removestr_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&removestr, 'f');
    if (this->line_no == -1)
        return (void)cmdlogrequired(&removestr, 'p');
    if (this->n == -1)
        return (void)cmdlogrequired(&removestr, 'n');
    if (this->direction == 0)
        return (void)cmdlog(&removestr, "one of \"->\" or \"-<\" is required");

    if (fu_backup(this->path) == -1)
        return (void)cmdlog(&removestr, "backup failed, ignoring: %s",
            strerror(errno));

    int pos = fu_pwhereat(
        this->path, this->line_no, this->col_no,
        this->direction, &this->n);
    if (pos == -1)
        return (void)cmdlog(&removestr, "not a valid position");
    if (fu_removeat(this->path, pos, this->n) == -1)
        return (void)cmdlog(&removestr, "remove failed: %s",
            strerror(errno));
    cmdlog(&removestr, "done");
}

const command removestr = {
    .name       = "removestr",
    .optstring  = ":f:p:n:<>",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run
};
