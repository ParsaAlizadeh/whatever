#include "cutstr.h"
#include "setup.h"

typedef struct {
    char *path;
    long line_no, col_no;
    long n;
    int direction;
} cutstr_t;

static void *make(void) {
    cutstr_t *this = malloc(sizeof(cutstr_t));
    this->path = NULL;
    this->line_no = this->col_no = this->n = -1;
    this->direction = 0;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    cutstr_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('f', path)
    SINGLE_OPTION_POSITION('p', cutstr, line_no, col_no)
    SINGLE_OPTION_SCANF('n', cutstr, n, -1, "%lu")
    SINGLE_OPTION_CONSTANT('>', direction, 0, 1)
    SINGLE_OPTION_CONSTANT('<', direction, 0, -1)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this) {
    cutstr_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&cutstr, 'f');
    if (this->line_no == -1)
        return (void)cmdlogrequired(&cutstr, 'p');
    if (this->n == -1)
        return (void)cmdlogrequired(&cutstr, 'n');
    if (this->direction == 0)
        return (void)cmdlog(&cutstr, "one of \"->\" or \"-<\" is required");

    if (fu_backup(this->path) == -1)
        return (void)cmdlog(&cutstr, "backup failed, ignoring: %s",
            strerror(errno));

    long pos = fu_pwhereat(
        this->path, this->line_no, this->col_no,
        this->direction, &this->n);
    if (pos == -1)
        return (void)cmdlog(&cutstr, "not a valid position");
    char *content = fu_preadat(this->path, pos, this->n);
    if (content == NULL)
        return (void)cmdlog(&cutstr, "can not read at the position");
    clipboard_set(content);
    if (fu_removeat(this->path, pos, this->n) == -1)
        return (void)cmdlog(&cutstr, "cut failed: %s",
            strerror(errno));
    cmdlog(&cutstr, "done");
}

const command cutstr = {
    .name       = "cutstr",
    .optstring  = ":f:p:n:<>",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run
};