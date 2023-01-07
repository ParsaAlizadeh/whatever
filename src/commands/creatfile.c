#include "creatfile.h"
#include "setup.h"

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
    if (this->path == NULL)
        return (void)cmdlogrequired(&creatfile, 'f');
    char *dir = fu_dirname(this->path);
    if (mkdir_p(dir) == -1)
        return (void)cmdlog(&creatfile, "failed to create directory: %s",
            strerror(errno));
    free(dir);
    if (fu_exists(this->path))
        return (void)cmdlog(&creatfile, "\"%s\" exists", this->path);
    FILE *file;
    if ((file = fopen(this->path, "w")) == NULL)
        return (void)cmdlog(&creatfile, "failed to create file: %s",
            strerror(errno));
    fclose(file);
    cmdlog(&creatfile, "done");
}

const command creatfile = {
    .name       = "creatfile",
    .optstring  = ":f:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run
};