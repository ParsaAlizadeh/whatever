#include "command.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "context.h"
#include "logging.h"

const command *lookup_command(char *name) {
    int n = ctx_get_ncmds();
    const command *cmds = ctx_get_allcmds();
    for (int i = 0; i < n; i++)
        if (strcmp(cmds[i].name, name) == 0)
            return &cmds[i];
    return NULL;
}

command_obj make_command(vector *tokens) {
    command_obj result = {0};
    if (tokens->size == 0)
        return result;
    result.cmd = lookup_command((char *)tokens->seq[0]);
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
            loginfo("%s: unknown option \"-%c\"", self.cmd->name, optopt);
            return CMD_FAILURE;
        case ':':
            loginfo("%s: option \"-%c\" requires an argument", self.cmd->name, optopt);
            return CMD_FAILURE;
        case 'h':
            loginfo("usage: %s %s", self.cmd->name, self.cmd->usage);
            break;
        default:
            int rc = self.cmd->set_opt(self.obj, opt, optarg);
            if (rc != CMD_SUCCESS) {
                switch (rc) {
                case CMD_REPEATED_OPTION:
                    loginfo("%s: option \"-%c\" is repeated",
                        self.cmd->name, opt);
                    break;
                case CMD_UNEXPECTED:
                    loginfo("%s: unexpected option \"-%c\"",
                        self.cmd->name, opt);
                    break;
                }
                return rc;
            }
            break;
        }
    }
    if (optind < argc) {
        loginfo("%s: extra arguments was found", self.cmd->name);
        return CMD_FAILURE;
    }
    return CMD_SUCCESS;
}

void run_command(command_obj self, char *inp, char **out) {
    self.cmd->run(self.obj, inp, out);
}

int procedure_command(vector *tokens, char *inp, char **out) {
    if (tokens == NULL || tokens->size == 0) {
        loginfo("procedure: tokens should be non empty");
        return CMD_FAILURE;
    }
    command_obj cobj = make_command(tokens);
    if (cobj.cmd == NULL) {
        loginfo("procedure: invalid command \"%s\"", (char *)tokens->seq[0]);
        return CMD_FAILURE;
    }
    int rc = feed_options(cobj, tokens);
    if (rc != CMD_SUCCESS) {
        cobj.cmd->free(cobj.obj);
        return rc;
    }

    /* may or maynot return error-code, didn't care, won't logged */
    (void) cobj.cmd->set_opt(cobj.obj, 'f', (char *)ctx_get());

    run_command(cobj, inp, out);
    cobj.cmd->free(cobj.obj);
    return CMD_SUCCESS;
}