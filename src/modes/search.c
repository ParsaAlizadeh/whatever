#include "search.h"

#include <stdlib.h>
#include "cboxi.h"
#include "../logging.h"
#include "../editor.h"
#include "../context.h"
#include "../pattern.h"
#include "../fileutil.h"

static pos_t prvcur;

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
    ed->acur = prvcur;
    editor_hl_next();
}

static void _reset(void) {
    editor_hl_reset();
    wclear(ed->cw);
    ed->acur = prvcur;
}

void search_mode(void) {
    prvcur = ed->acur;
    cboxi_init('/');
    int running = 1;
    while (running) {
        ctx_set_edmode("SEARCH");
        int rc = cboxi_refresh();
        switch (rc) {
        case CBOXI_CANCEL:
            _reset();
            running = 0;
            break;
        case CBOXI_DONE:
            if (cboxi_size() == 0)
                _reset();
            running = 0;
            break;
        default:
            _update_hls();
        }
    }
    cboxi_end();
}
