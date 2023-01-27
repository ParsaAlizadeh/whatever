#include "legacy.h"

#include <stdio.h>
#include "../parse.h"
#include "../chain.h"

void legacy_mode(int n_cmds, const command all_cmds[]) {
    while (!feof(stdin)) {
        printf("$ ");
        fflush(stdout);
        vector *tokens = scan_line(stdin);
        if (tokens->size > 0)
            procedure_chain(n_cmds, all_cmds, tokens, NULL);
        vector_freeall(tokens);
    }
}
