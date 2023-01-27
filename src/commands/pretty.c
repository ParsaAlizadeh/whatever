#include "pretty.h"
#include "setup.h"

typedef struct {
    char *path;
} pretty_t;

static void *make(void) {
    pretty_t *this = malloc(sizeof(pretty_t));
    this->path = NULL;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    pretty_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('f', path)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this, char *inp, char **_out) {
    (void)inp;
    pretty_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&pretty, 'f');
    FILE *file = fu_open(this->path, "r");
    if (file == NULL)
        return (void)cmdlog(&pretty, "failed to open file: %s",
            strerror(errno));
    string *out = string_using(_out);
    prettify(file, out->f);
    string_free(out);
    fclose(file);
}

const command pretty = {
    .name       = "pretty",
    .optstring  = "f:",
    .usage      = "-f FILE",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};
