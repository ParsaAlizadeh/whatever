#include "cat.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../fileutil.h"

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
    if (this->path == NULL) {
        fprintf(stderr, "cat: option \"-f\" is required\n");
        return;
    }
    FILE *file;
    if ((file = fopen(this->path, "r")) == NULL) {
        fprintf(stderr, "cat: failed to open file: %s\n", strerror(errno));
        return;
    }
    fu_copy(file, stdout);
    fclose(file);
    fprintf(stderr, "cat: done\n");
    return;
}

const command cat = {
    .name       = "cat",
    .optstring  = ":f:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run
};