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

static int set_opt(void *_this, int c, char *argv) {
    cat_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('f', path)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this, char *inp, char **out) {
    (void)inp;
    cat_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&cat, 'f');
    FILE *file;
    if ((file = fu_open(this->path, "r")) == NULL)
        return (void)cmdlog(&cat, "failed to open file: %s", strerror(errno));
    size_t out_size;
    FILE *fout = open_memstream(out, &out_size);
    fu_copy(file, fout);
    fclose(file);
    fclose(fout);
    cmdlog(&cat, "done");
}

const command cat = {
    .name       = "cat",
    .optstring  = ":f:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};
