#include "buffer.h"

#include <unistd.h>
#include "../context.h"
#include "../fileutil.h"

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

int editor_readstr(char *str) {
    vecline *vc = vc_newstr(str);
    if (vc == NULL)
        return -1;
    editor_setvc(vc);
    return 0;
}

int editor_writepath(const char *path) {
    return vc_writepath(ed->vc, path);
}

int editor_readpath(const char *path) {
    if (path == NULL)
        return -1;
    vecline *vc = vc_newpath(path);
    if (vc == NULL)
        return -1;
    editor_setvc(vc);
    return 0;
}

int editor_readctx(void) {
    const char *ctx = ctx_get();
    if (ctx == NULL)
        return -1;
    return editor_readpath(ctx);
}

int editor_writectx(void) {
    const char *ctx = ctx_get();
    if (ctx == NULL)
        return -1;
    fu_backup(ctx);
    return editor_writepath(ctx);
}

void editor_unlinkbuffer(void) {
    unlink(BUFFER_PATH);
}
