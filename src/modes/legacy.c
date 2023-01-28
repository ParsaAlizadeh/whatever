#include "legacy.h"

#include <stdio.h>
#include "../parse.h"
#include "../chain.h"

void legacy_mode(void) {
    while (!feof(stdin)) {
        printf("$ ");
        fflush(stdout);
        vector *tokens = scan_line(stdin);
        if (tokens->size > 0)
            procedure_chain(tokens, NULL);
        vector_freeall(tokens);
    }
}
