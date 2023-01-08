#include <stdio.h>
#include <stdlib.h>
#include "src/parse.h"
#include "src/command.h"

#include "src/commands/creatfile.h"
#include "src/commands/insertstr.h"
#include "src/commands/removestr.h"
#include "src/commands/cat.h"
#include "src/commands/undo.h"
#include "src/commands/copystr.h"
#include "src/commands/cutstr.h"
#include "src/commands/pastestr.h"

int main() {
    const command all_cmds[] = {
        creatfile,
        insertstr,
        removestr,
        cat,
        undo,
        copystr,
        cutstr,
        pastestr
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