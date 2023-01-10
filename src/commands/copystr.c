#include "copystr.h"
#include "setup.h"

typedef struct {
    char *path;
    long line_no, col_no;
    long n;
    int direction;
} copystr_t;

static void *make(void) {
    copystr_t *this = malloc(sizeof(copystr_t));
    this->path = NULL;
    this->line_no = this->col_no = -1;
    this->n = -1;
    this->direction = 0;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    copystr_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('f', path)
    SINGLE_OPTION_CONSTANT('>', direction, 0, 1)
    SINGLE_OPTION_CONSTANT('<', direction, 0, -1)
    SINGLE_OPTION_POSITION('p', copystr, line_no, col_no)
    SINGLE_OPTION_SCANF('n', copystr, n, -1, "%lu")
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this, char *inp, char **out) {
    (void)inp;
    (void)out;
    copystr_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&copystr, 'f');
    if (this->line_no == -1)
        return (void)cmdlogrequired(&copystr, 'p');
    if (this->n == -1)
        return (void)cmdlogrequired(&copystr, 'n');
    if (this->direction == 0)
        return (void)cmdlog(&copystr, "one of \"->\" or \"-<\" is required");
    long pos = fu_pwhereat(
        this->path, this->line_no, this->col_no,
        this->direction, &this->n);
    if (pos == -1)
        return (void)cmdlog(&copystr, "can not locate the position");
    char *content = fu_preadat(this->path, pos, this->n);
    if (content == NULL)
        return (void)cmdlog(&copystr, "can not read at the position");
    clipboard_set(content);
    cmdlog(&copystr, "done");
}

const command copystr = {
    .name       = "copystr",
    .optstring  = ":f:p:n:<>",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};
