#include "creatfile.h"

#include <stdlib.h>
#include <stdio.h>
#include "string.h"

typedef struct {
    char *file;
} creatfile_t;

static void *make(void) {
    creatfile_t *this = malloc(sizeof(creatfile));
    this->file = NULL;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    creatfile_t *this = _this;
    switch (c) {
    case 'f':
        this->file = argv;
        break;
    default:
        fprintf(stderr, "creatfile: unexpected flag %c\n", c);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static void run(void *_this) {
    creatfile_t *this = _this;
    if (this->file == NULL) {
        fprintf(stderr, "creatfile: option \"-f\" is required\n");
        return;
    }
    fprintf(stderr, "dry-run creatfile file=[[%s]]\n", this->file);
}

const command creatfile = {
    .name       = "creatfile",
    .optstring  = ":f:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run
};