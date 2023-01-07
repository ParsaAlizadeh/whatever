#include <stdio.h>
#include <stdlib.h>
#include "parse.h"
#include "command.h"
#include "commands/creatfile.h"

int main() {
    const command all_cmds[] = {
        creatfile
    };
    int n_cmds = sizeof(all_cmds) / sizeof(command);
    while (1) {
        vector *tokens = scan_line();
        if (tokens->size == 0)
            break;
        procedure_command(n_cmds, all_cmds, tokens);
        vector_freeall(tokens);
    }
    return EXIT_SUCCESS;
}