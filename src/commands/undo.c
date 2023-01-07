#include "undo.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../fileutil.h"

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
    if (this->path == NULL) {
        fprintf(stderr, "undo: option \"-f\" is required\n");
        return;
    }
    char *bakpath = fu_backuppath(this->path);
    if (!fu_exists(bakpath)) {
        fprintf(stderr, "undo: no backup exists\n");
        return;
    }
    if (rename(bakpath, this->path) == -1) {
        fprintf(stderr, "undo: rename failed: %s", strerror(errno));
        return;
    }
    fprintf(stderr, "undo: done\n");
    return;
}

const command undo = {
    .name       = "undo",
    .optstring  = ":f:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run
};