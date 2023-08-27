#include "help.h"
#include "setup.h"

#include "../context.h"

static void *make(void) {
    return NULL;
}

static int set_opt(void *_this, int c, char *argv) {
    (void)_this;
    (void)c;
    (void)argv;
    return CMD_UNEXPECTED;
}

static void run(void *_this, char *inp, char **_out) {
    (void)_this;
    (void)inp;
    string *out = string_using(_out);
    for (int i = 0; i < ctx_ncmds; i++) {
        const command *cmd = &ctx_allcmds[i];
        fprintf(out->f, "%s %s\n", cmd->name, cmd->usage);
    }
    string_free(out);
}

const command help = {
    .name       = "help",
    .optstring  = "",
    .usage      = "",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};
