#include "creatfile.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../fileutil.h"

typedef struct {
    char *path;
} creatfile_t;

static void *make(void) {
    creatfile_t *this = malloc(sizeof(creatfile_t));
    this->path = NULL;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    creatfile_t *this = _this;
    switch (c) {
    case 'f':
        if (this->path != NULL)
            return CMD_REPEATED_OPTION;
        this->path = argv;
        break;
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this) {
    creatfile_t *this = _this;
    if (this->path == NULL) {
        fprintf(stderr, "creatfile: option \"-f\" is required\n");
        return;
    }
    char *dir = fu_dirname(this->path);
    if (mkdir_p(dir) == -1) {
        fprintf(stderr, "creatfile: failed to create directory: %s\n",
            strerror(errno));
        return;
    }
    free(dir);
    if (fu_exists(this->path)) {
        fprintf(stderr, "creatfile: \"%s\" exists\n", this->path);
        return;
    }
    FILE *file;
    if ((file = fopen(this->path, "w")) == NULL) {
        fprintf(stderr, "creatfile: failed to create file \"%s\": %s\n",
            this->path, strerror(errno));
        return;
    }
    fclose(file);
    fprintf(stderr, "creatfile: done\n");
}

const command creatfile = {
    .name       = "creatfile",
    .optstring  = ":f:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run
};