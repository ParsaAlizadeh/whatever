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
    static char optstringbuf[256];

    int argc = tokens->size;
    char **argv = (char **)tokens->seq;
    int opt;

    /* start from beginning, optind=0 forces getopt to reset its internal variables */
    optind = 0;
    /* disable print errors */
    opterr = 0;

    snprintf(optstringbuf, sizeof(optstringbuf),
        ":h%s", self.cmd->optstring);

    while ((opt = getopt(argc, argv, optstringbuf)) != -1) {
        switch (opt) {
        case '?':
            fprintf(stderr, "%s: unknown option \"-%c\"\n", self.cmd->name, optopt);
            return CMD_FAILURE;
        case ':':
            fprintf(stderr, "%s: option \"-%c\" requires an argument\n", self.cmd->name, optopt);
            return CMD_FAILURE;
        case 'h':
            fprintf(stderr, "usage: %s %s\n", self.cmd->name, self.cmd->usage);
            break;
        default:
            int rc = self.cmd->set_opt(self.obj, opt, optarg);
            if (rc != CMD_SUCCESS) {
                switch (rc) {
                case CMD_REPEATED_OPTION:
                    fprintf(stderr, "%s: option \"-%c\" is repeated\n",
                        self.cmd->name, opt);
                    break;
                case CMD_UNEXPECTED:
                    fprintf(stderr, "%s: unexpected option \"-%c\"\n",
                        self.cmd->name, opt);
                    break;
                }
                return rc;
            }
            break;
        }
    }
    if (optind < argc) {
        fprintf(stderr, "%s: extra arguments was found\n", self.cmd->name);
        return CMD_FAILURE;
    }
    return CMD_SUCCESS;
}

void run_command(command_obj self, char *inp, char **out) {
    self.cmd->run(self.obj, inp, out);
}

int procedure_command(int n, const command cmds[], vector *tokens, char *inp, char **out) {
    if (tokens == NULL || tokens->size == 0) {
        fprintf(stderr, "procedure: tokens should be non empty\n");
        return CMD_FAILURE;
    }
    command_obj cobj = make_command(n, cmds, tokens);
    if (cobj.cmd == NULL) {
        fprintf(stderr, "procedure: invalid command \"%s\"\n", (char *)tokens->seq[0]);
        return CMD_FAILURE;
    }
    int rc = feed_options(cobj, tokens);
    if (rc != CMD_SUCCESS) {
        cobj.cmd->free(cobj.obj);
        return rc;
    }
    run_command(cobj, inp, out);
    cobj.cmd->free(cobj.obj);
    return CMD_SUCCESS;
}