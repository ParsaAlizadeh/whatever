#include "run.h"

#include <stdlib.h>
#include "../vecstr.h"
#include "../context.h"
#include "../chain.h"
#include "../parse.h"

void editor_run_init(void) {
    editor_hl_reset();
    ctx_set_buf_mode(1);
    ctx_save();
}

void editor_run_end(run_mode_t mode, char *out) {
    if (mode == RUN_READWRITE && ctx_get_buf_mode()) {
        editor_hl_reset();
        ed->modified = 1;
        editor_loadctx();
    }
    ctx_set_buf_mode(0);
    if (out != NULL) {
        ctx_set(NULL);
        editor_setvc(vc_newstr(out));
        editor_reset();
        free(out);
    }
}

void editor_run(run_mode_t mode, vector *tokens) {
    editor_run_init();
    char *out = NULL;
    procedure_chain(tokens, &out);
    editor_run_end(mode, out);
}

void editor_runf(run_mode_t mode, const char *format, ...) {
    string *cmd = string_new();
    va_list args;
    va_start(args, format);
    vfprintf(cmd->f, format, args);
    va_end(args);
    char *strcmd = string_free(cmd);
    vector *tokens = scan_strline(strcmd);
    editor_run(mode, tokens);
    vector_freeall(tokens);
    free(strcmd);
}
