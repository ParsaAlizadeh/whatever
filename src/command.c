#include "command.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

const command *lookup_command(int n, const command cmds[], char *name) {
    for (int i = 0; i < n; i++)
        if (strcmp(cmds[i].name, name) == 0)
            return &cmds[i];
    return NULL;
}

command_obj make_command(int n, const command cmds[], vector *tokens) {
    command_obj result = {0};
    if (tokens->size == 0)
        return result;
    result.cmd = lookup_command(n, cmds, (char *)tokens->seq[0]);
    if (result.cmd == NULL)
        return result;
    result.obj = result.cmd->make();
    return result;
}

void feed_options(command_obj self, vector *tokens) {
    int argc = tokens->size;
    char **argv = (char **)tokens->seq;
    int opt;

    /* start from beginning, optind=0 forces getopt to reset its internal variables */
    optind = 0;
    /* disable print errors */
    opterr = 0;

    while ((opt = getopt(argc, argv, self.cmd->optstring)) != -1) {
        switch (opt) {
        case '?':
            fprintf(stderr, "%s: unknown option %c\n", self.cmd->name, optopt);
            break;
        case ':':
            fprintf(stderr, "%s: option %c requires an argument\n", self.cmd->name, optopt);
            break;
        default:
            self.cmd->set_opt(self.obj, opt, optarg);
            break;
        }
    }
    if (optind < argc)
        fprintf(stderr, "%s: ignore extra arguments\n", self.cmd->name);
}

void run_command(command_obj self) {
    self.cmd->run(self.obj);
}

void procedure_command(int n, const command cmds[], vector *tokens) {
    if (tokens == NULL || tokens->size == 0) {
        fprintf(stderr, "procedure: tokens should be non empty\n");
        return;
    }
    command_obj cobj = make_command(n, cmds, tokens);
    if (cobj.cmd == NULL) {
        fprintf(stderr, "procedure: invalid command %s\n", (char *)tokens->seq[0]);
        return;
    }
    feed_options(cobj, tokens);
    run_command(cobj);
}