#include "command.h"

#include <stdlib.h>
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

void command_mode(void) {
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
        editor_run_command(tokens);
    else
        wclear(ed->cw);
    vector_freeall(tokens);
    cboxi_end();
}
