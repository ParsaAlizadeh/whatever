#include "findpat.h"
#include "setup.h"

typedef struct {
    char *path;
    char *patstr;
    pattern *pat;
    long count;
    long at;
    int word;
    int all;
} findpat_t;

static void *make(void) {
    findpat_t *this = malloc(sizeof(findpat_t));
    this->path = this->patstr = NULL;
    this->pat = NULL;
    this->count = this->at = -1;
    this->word = this->all = 0;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    findpat_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('f', path)
    SINGLE_OPTION_ARGV('s', patstr)
    SINGLE_OPTION_CONSTANT('c', count, -1, 0)
    SINGLE_OPTION_SCANF_COND('i', findpat, at, -1, "%ld", this->at > 0)
    SINGLE_OPTION_CONSTANT('w', word, 0, 1)
    SINGLE_OPTION_CONSTANT('a', all, 0, 1)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this, char *inp, char **_out) {
    findpat_t *this = _this;
    if (this->path == NULL)
        return (void)cmdlogrequired(&findpat, 'f');
    if (this->patstr == NULL)
        this->patstr = inp;
    if (this->patstr == NULL)
        return (void)cmdlogrequired(&findpat, 's');
    this->pat = pattern_new(this->patstr);
    if (this->pat == NULL)
        return (void)cmdlog(&findpat, "invalid pattern");
    if (this->at != -1 && this->all)
        return (void)cmdlog(&findpat, "\"-i\" and \"-a\" are in conflict");
    if (this->at == -1 && !this->all)
        this->at = 1;
    FILE *file = fu_open(this->path, "r");
    if (file == NULL)
        return (void)cmdlog(&findpat, "file not exists");
    string *out = string_using(_out);
    int needcomma = 0;
    subseq_t ss;
    while ((ss = fu_nextmatch(file, this->pat)).offset != -1) {
        if (this->at != -1)
            this->at--;
        if (this->at > 0)
            continue;
        if (this->count != -1)
            this->count++;
        else {
            long pos = ss.offset;
            if (this->word)
                pos = fu_wordat(file, pos);
            if (needcomma)
                fprintf(out->f, ", ");
            fprintf(out->f, "%ld", pos);
            needcomma = 1;
        }
        if (this->at == 0)
            break;
    }
    if (this->count != -1)
        fprintf(out->f, "%ld\n", this->count);
    else if (!needcomma)
        fprintf(out->f, "-1\n");
    else
        fprintf(out->f, "\n");
    fclose(file);
    string_free(out);
    cmdlog(&findpat, "done");
}

static void findpat_free(void *_this) {
    findpat_t *this = _this;
    if (this->pat != NULL)
        pattern_free(this->pat);
    free(this);
}

const command findpat = {
    .name       = "find",
    .optstring  = "f:s:ci:wa",
    .usage      = "-f FILE -s PATTERN [-cwa] [-i AT]",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = findpat_free
};
