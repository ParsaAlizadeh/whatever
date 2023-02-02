#include "run.h"

#include <stdlib.h>
#include "../vecstr.h"
#include "../context.h"
#include "../chain.h"
#include "../parse.h"

void editor_run_init(void) {
    ctx_mode = CTX_BUFFER;
    editor_writepath(ctx_get());
    ctx_counter = 0;
}

void editor_run_end(char *out) {
    if (ctx_counter > 0) {
        editor_hl_reset();
        ed->modified = 1;
        editor_readctx();
    }
    ctx_fix();
    if (out != NULL) {
        editor_writectx();
        ctx_set(CTX_NULL, NULL);
        editor_readstr(out);
        editor_reset();
    }
}

void editor_run(vector *tokens) {
    editor_run_init();
    char *out = NULL;
    procedure_chain(tokens, &out);
    editor_run_end(out);
    free(out);
}

void editor_runf(const char *format, ...) {
    string *cmd = string_new();
    va_list args;
    va_start(args, format);
    vfprintf(cmd->f, format, args);
    va_end(args);
    char *strcmd = string_free(cmd);
    vector *tokens = scan_strline(strcmd);
    editor_run(tokens);
    vector_freeall(tokens);
    free(strcmd);
}
