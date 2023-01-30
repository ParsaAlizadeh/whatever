#include "buffer.h"

#include <unistd.h>
#include "../context.h"

void editor_reset() {
    ed->asel = (pos_t) { -1, 0 };
    ed->modified = 0;
    editor_mvcur();
    editor_hl_reset();
}

void editor_setvc(vecline *vc) {
    vc_free(ed->vc);
    ed->vc = vc;
}

int editor_saveas(const char *path) {
    return vc_writepath(ed->vc, path);
}

int editor_loadctx(void) {
    vecline *vc = vc_newpath(ctx_get());
    if (vc == NULL)
        return -1;
    editor_setvc(vc);
    return 0;
}

void editor_unlinkbuffer(void) {
    unlink(BUFFER_PATH);
}
