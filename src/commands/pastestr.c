#include "pastestr.h"
#include "setup.h"

#include "insertstr.h"

typedef struct {
    void *insertobj;
} pastestr_t;

static void *make(void) {
    pastestr_t *this = malloc(sizeof(pastestr_t));
    this->insertobj = insertstr.make();
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    pastestr_t *this = _this;
    int rc;
    if ((rc = insertstr.set_opt(this->insertobj, c, argv)) != CMD_SUCCESS)
        return rc;
    return CMD_SUCCESS;
}

static void run(void *_this) {
    pastestr_t *this = _this;
    if (insertstr.set_opt(this->insertobj, 's', (char *)clipboard_get()) != CMD_SUCCESS)
        return;
    insertstr.run(this->insertobj);
    cmdlog(&pastestr, "done");
}

static void pastestr_free(void *_this) {
    pastestr_t *this = _this;
    insertstr.free(this->insertobj);
    free(this);
}

const command pastestr = {
    .name       = "pastestr",
    .optstring  = ":f:p:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = pastestr_free
};