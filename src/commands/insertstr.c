#include "insertstr.h"
#include "setup.h"

typedef struct {
    char *path;
    char *str;
    long line_no, col_no;
} insertstr_t;

static void *make(void) {
    insertstr_t *this = malloc(sizeof(insertstr_t));
    this->path = this->str = NULL;
    this->line_no = this->col_no = -1;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    insertstr_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('f', path)
    SINGLE_OPTION_ARGV('s', str)
    SINGLE_OPTION_POSITION('p', insertstr, line_no, col_no)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this, char *inp, char **out) {
    (void)out;
    insertstr_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&insertstr, 'f');
    if (this->str == NULL)
        this->str = inp;
    if (this->str == NULL)
        return (void)cmdlogrequired(&insertstr, 's');
    if (this->line_no == -1)
        return (void)cmdlogrequired(&insertstr, 'p');

    long pos = fu_pwhereat(this->path, this->line_no, this->col_no, 1, NULL);
    if (pos == -1)
        return (void)cmdlog(&insertstr, "not a valid position");
    if (fu_backup(this->path) == -1)
        cmdlog(&insertstr, "backup failed, ignoring: %s",
            strerror(errno));
    if (fu_insertat(this->path, pos, this->str) == -1)
        return (void)cmdlog(&insertstr, "insert failed: %s",
            strerror(errno));
}

const command insertstr = {
    .name       = "insert",
    .optstring  = "f:s:p:",
    .usage      = "[-f FILE] -p LINE:COL [-s STRING]",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};
