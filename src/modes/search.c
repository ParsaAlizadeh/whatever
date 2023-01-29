#include "search.h"

#include <stdlib.h>
#include "cboxi.h"
#include "../logging.h"
#include "../editor.h"
#include "../context.h"
#include "../pattern.h"
#include "../fileutil.h"

static void _update_hls(void) {
    editor_hl_reset();
    pattern *pat = pattern_new(cboxi_content());
    if (pat == NULL)
        return;
    editor_run_init();
    FILE *file = fu_open(ctx_get(), "r");
    subseq_t ss;
    while ((ss = fu_nextmatch(file, pat)).offset != -1)
        editor_hl_addt(ss.offset, ss.size);
    pattern_free(pat);
    fclose(file);
    ctx_set_buf_mode(0);
    editor_run_end(NULL);
}

void search_mode(void) {
    cboxi_init('/');
    int running = 1;
    while (running) {
        ctx_set_edmode("SEARCH");
        int rc = cboxi_refresh();
        switch (rc) {
        case CBOXI_CANCEL:
            editor_hl_reset();
            wclear(ed->cw);
            running = 0;
            break;
        case CBOXI_DONE:
            running = 0;
            break;
        default:
            _update_hls();
        }
    }
    cboxi_end();
}
