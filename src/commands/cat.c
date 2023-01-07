#include "cat.h"
#include "setup.h"

typedef struct {
    char *path;
} cat_t;

static void *make(void) {
    cat_t *this = malloc(sizeof(cat_t));
    this->path = NULL;
    return this;
}

static int set_opt(void *_this, int c, char *arg) {
    cat_t *this = _this;
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
    cat_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&cat, 'f');
    FILE *file;
    if ((file = fopen(this->path, "r")) == NULL)
        return (void)cmdlog(&cat, "failed to open file: %s", strerror(errno));
    fu_copy(file, stdout);
    fclose(file);
    cmdlog(&cat, "done");
}

const command cat = {
    .name       = "cat",
    .optstring  = ":f:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run
};