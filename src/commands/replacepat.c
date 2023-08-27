#include "replacepat.h"
#include "setup.h"

typedef struct {
    char *path;
    char *patstr;
    char *repstr;
    pattern *pat;
    long at;
    int all;
} replacepat_t;

static void *make(void) {
    replacepat_t *this = malloc(sizeof(replacepat_t));
    this->path = this->patstr = this->repstr = NULL;
    this->pat = NULL;
    this->at = -1;
    this->all = 0;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    replacepat_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('f', path)
    SINGLE_OPTION_ARGV('s', patstr)
    SINGLE_OPTION_ARGV('r', repstr)
    SINGLE_OPTION_SCANF_COND('i', replacepat, at, -1, "%ld", this->at > 0)
    SINGLE_OPTION_CONSTANT('a', all, 0, 1)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this, char *inp, char **out) {
    (void)out;
    replacepat_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&replacepat, 'f');
    if (this->patstr == NULL)
        this->patstr = inp;
    if (this->patstr == NULL)
        return (void)cmdlogrequired(&replacepat, 's');
    if (this->repstr == NULL)
        this->repstr = "";
    this->pat = pattern_new(this->patstr);
    if (this->pat == NULL)
        return (void)cmdlog(&replacepat, "invalid pattern");
    if (this->at != -1 && this->all)
        return (void)cmdlog(&replacepat, "\"-i\" and \"-a\" are in conflict");
    if (this->at == -1 && !this->all)
        this->at = 1;
    FILE *file = fu_open(this->path, "r");
    if (file == NULL)
        return (void)cmdlog(&replacepat, "failed to open file: %s",
            strerror(errno));
    long replaced = 0;
    subseq_t ss;
    int repsize = strlen(this->repstr);
    while ((ss = fu_nextmatch(file, this->pat)).offset != -1) {
        if (this->at != -1)
            this->at--;
        if (this->at > 0)
            continue;
        if (!replaced && strcmp(this->path, BUFFER_PATH) == 0)
            editor_set_tcur(ss.offset);
        if (!replaced && fu_backup(this->path) == -1)
            cmdlog(&replacepat, "backup failed, ignoring: %s",
                strerror(errno));
        fclose(file);
        if (fu_removeat(this->path, ss.offset, ss.size) == -1)
            return (void)cmdlog(&replacepat, "remove failed");
        if (fu_insertat(this->path, ss.offset, this->repstr) == -1)
            return (void)cmdlog(&replacepat, "insert failed");
        file = fu_open(this->path, "r");
        if (file == NULL)
            return (void)cmdlog(&replacepat, "failed to open file: %s",
                strerror(errno));
        pattern_reset(this->pat);
        fu_copyn(file, NULL, ss.offset+repsize);
        this->pat->visited = ss.offset+repsize;
        replaced++;
        if (this->at == 0)
            break;
    }
    fclose(file);
    if (!replaced)
        cmdlog(&replacepat, "pattern not found, not touching the file");
}

static void replacepat_free(void *_this) {
    replacepat_t *this = _this;
    if (this->pat != NULL)
        pattern_free(this->pat);
    free(this);
}

const command replacepat = {
    .name       = "replace",
    .optstring  = "f:s:r:i:a",
    .usage      = "[-f FILE] -s PATTERN -r REPLACE [-a] [-i AT]",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = replacepat_free
};
