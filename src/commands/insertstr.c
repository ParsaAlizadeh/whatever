#include "insertstr.h"
#include "setup.h"

typedef struct {
    char *path;
    char *str;
    int line_no, col_no;
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
    case 'f':
        if (this->path != NULL)
            return CMD_REPEATED_OPTION;
        this->path = argv;
        break;
    case 's':
        if (this->str != NULL)
            return CMD_REPEATED_OPTION;
        this->str = argv;
        break;
    case 'p':
        if (this->line_no != -1)
            return CMD_REPEATED_OPTION;
        if (sscanf(argv, "%u:%u", &this->line_no, &this->col_no) < 2) {
            cmdlog(&insertstr, "option \"-p\" must be in this format \"%%u:%%u\"");
            return CMD_FAILURE;
        }
        break;
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this) {
    insertstr_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&insertstr, 'f');
    if (this->str == NULL)
        return (void)cmdlogrequired(&insertstr, 's');
    if (this->line_no == -1)
        return (void)cmdlogrequired(&insertstr, 'p');

    if (fu_backup(this->path) == -1)
        cmdlog(&insertstr, "backup failed, ignoring: %s",
            strerror(errno));

    FILE *file;
    if ((file = fopen(this->path, "r")) == NULL)
        return (void)cmdlog(&insertstr, "unable to open file: %s",
            strerror(errno));
    int pos = fu_whereat(file, this->line_no, this->col_no);
    fclose(file);
    if (pos == -1)
        return (void)cmdlog(&insertstr, "not a valid position");
    if (fu_insertat(this->path, pos, this->str) == -1)
        return (void)cmdlog(&insertstr, "insert failed: %s",
            strerror(errno));
    cmdlog(&insertstr, "done");
}

const command insertstr = {
    .name       = "insertstr",
    .optstring  = ":f:s:p:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run
};