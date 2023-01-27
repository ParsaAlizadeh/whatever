#include "command.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "cboxi.h"
#include "../logging.h"
#include "../editor.h"
#include "../context.h"
#include "../fileutil.h"
#include "../parse.h"
#include "../chain.h"

static FILE *_read_command(void) {
    while (1) {
        ctx_set_edmode("COMMAND");
        int rc = cboxi_refresh();
        if (rc == CBOXI_CANCEL)
            return NULL;
        if (rc == CBOXI_DONE)
            break;
    }
    return cboxi_content();
}

static time_t _ctx_mtime(void) {
    struct stat buf;
    stat(ctx_get(), &buf);
    return buf.st_mtime;
}

static void _run_command(int n_cmds, const command all_cmds[], vector *tokens) {
    ctx_set_buf_mode(1);
    ctx_save();
    time_t before = _ctx_mtime();
    char *out = NULL;
    procedure_chain(n_cmds, all_cmds, tokens, &out);
    time_t after = _ctx_mtime();
    if (after > before) {
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

void command_mode(int n_cmds, const command all_cmds[]) {
    cboxi_init(':');
    FILE *inp = _read_command();
    if (inp == NULL) {
        wclear(ed->cw);
        cboxi_end();
        return;
    }
    vector *tokens = scan_line(inp);
    fclose(inp);
    if (tokens->size > 0)
        _run_command(n_cmds, all_cmds, tokens);
    else
        wclear(ed->cw);
    vector_freeall(tokens);
    cboxi_end();
}
