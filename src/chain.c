#include "chain.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"

int procedure_chain(vector *tokens, char **out) {
    vector *chain = vector_new();
    vector *cur = vector_new();
    for (int i = 0; i < tokens->size; i++) {
        char *part = tokens->seq[i];
        if (strcmp(part, PIPENCODE) == 0) {
            vector_push(chain, cur);
            cur = vector_new();
            continue;
        }
        vector_push(cur, part);
    }
    vector_push(chain, cur);
    char *current = NULL, *nxt = NULL;
    int rc = CMD_SUCCESS;
    for (int i = 0; i < chain->size; i++) {
        vector *local_tokens = chain->seq[i];
        int rc = procedure_command(local_tokens, current, &nxt);
        if (current != NULL)
            free(current);
        current = nxt;
        nxt = NULL;
        if (rc != CMD_SUCCESS)
            break;
    }
    for (int i = 0; i < chain->size; i++) {
        vector *local_tokens = chain->seq[i];
        vector_free(local_tokens);
    }
    vector_free(chain);
    if (out != NULL) {
        *out = current;
    } else if (current != NULL) {
        printf("%s", current);
        free(current);
    }
    return rc;
}
