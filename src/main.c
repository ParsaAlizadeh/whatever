#include <stdio.h>
#include <stdlib.h>
#include "parse.h"
#include "command.h"

#include "commands/creatfile.h"
#include "commands/insertstr.h"
#include "commands/cat.h"
#include "commands/undo.h"

int main() {
    const command all_cmds[] = {
        creatfile,
        insertstr,
        cat,
        undo
    };
    int n_cmds = sizeof(all_cmds) / sizeof(command);
    while (!feof(stdin)) {
        vector *tokens = scan_line();
        if (tokens->size == 0) /* empty line */
            continue;
        procedure_command(n_cmds, all_cmds, tokens);
        vector_freeall(tokens);
    }
    return EXIT_SUCCESS;
}