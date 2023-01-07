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

int feed_options(command_obj self, vector *tokens) {
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
            fprintf(stderr, "%s: unknown option \"-%c\"\n", self.cmd->name, optopt);
            return EXIT_FAILURE;
        case ':':
            fprintf(stderr, "%s: option \"-%c\" requires an argument\n", self.cmd->name, optopt);
            return EXIT_FAILURE;
        default:
            if (self.cmd->set_opt(self.obj, opt, optarg))
                return EXIT_FAILURE;
            break;
        }
    }
    if (optind < argc) {
        fprintf(stderr, "%s: extra arguments was found\n", self.cmd->name);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void run_command(command_obj self) {
    self.cmd->run(self.obj);
}

int procedure_command(int n, const command cmds[], vector *tokens) {
    if (tokens == NULL || tokens->size == 0) {
        fprintf(stderr, "procedure: tokens should be non empty\n");
        return EXIT_FAILURE;
    }
    command_obj cobj = make_command(n, cmds, tokens);
    if (cobj.cmd == NULL) {
        fprintf(stderr, "procedure: invalid command \"%s\"\n", (char *)tokens->seq[0]);
        return EXIT_FAILURE;
    }
    int rc;
    if ((rc = feed_options(cobj, tokens)))
        return rc;
    run_command(cobj);
    return EXIT_SUCCESS;
}