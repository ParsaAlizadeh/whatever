#include "cutstr.h"
#include "setup.h"

#include "copystr.h"
#include "removestr.h"

typedef struct {
    void *copyobj;
    void *removeobj;
} cutstr_t;

static void *make(void) {
    cutstr_t *this = malloc(sizeof(cutstr_t));
    this->copyobj = copystr.make();
    this->removeobj = removestr.make();
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    cutstr_t *this = _this;
    int rc;
    if ((rc = copystr.set_opt(this->copyobj, c, argv)) != CMD_SUCCESS)
        return rc;
    if ((rc = removestr.set_opt(this->removeobj, c, argv)) != CMD_SUCCESS)
        return rc;
    return CMD_SUCCESS;
}

static void run(void *_this, char *inp, char **out) {
    cutstr_t *this = _this;
    copystr.run(this->copyobj, inp, out);
    removestr.run(this->removeobj, inp, out);
    cmdlog(&cutstr, "done");
}

static void cutstr_free(void *_this) {
    cutstr_t *this = _this;
    copystr.free(this->copyobj);
    removestr.free(this->removeobj);
    free(this);
}

const command cutstr = {
    .name       = "cutstr",
    .optstring  = ":f:p:n:<>",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = cutstr_free
};