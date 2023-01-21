#include "pastestr.h"
#include "setup.h"

typedef struct {
    char *path;
    long line_no, col_no;
} pastestr_t;

static void *make(void) {
    pastestr_t *this = malloc(sizeof(pastestr_t));
    this->path = NULL;
    this->line_no = this->col_no = -1;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    pastestr_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('f', path)
    SINGLE_OPTION_POSITION('p', pastestr, line_no, col_no)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this, char *inp, char **out) {
    (void)inp;
    (void)out;
    pastestr_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&pastestr, 'f');
    if (this->line_no == -1)
        return (void)cmdlogrequired(&pastestr, 'p');
    if (fu_backup(this->path) == -1)
        cmdlog(&pastestr, "backup failed, ignoring: %s",
            strerror(errno));
    long pos = fu_pwhereat(this->path, this->line_no, this->col_no, 1, NULL);
    if (pos == -1)
        return (void)cmdlog(&pastestr, "not a valid position");
    if (fu_finsertat(this->path, pos, clipboard_get()) == -1)
        return (void)cmdlog(&pastestr, "insert failed: %s",
            strerror(errno));
    cmdlog(&pastestr, "done");
}

const command pastestr = {
    .name       = "paste",
    .optstring  = "f:p:",
    .usage      = "-f FILE -p LINE:COL",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};
