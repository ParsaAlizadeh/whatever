#include "grepat.h"
#include "setup.h"

typedef struct {
    vector *files;
    char *patstr;
    pattern *pat;
    long count;
    int all;
} grepat_t;

static void *make(void) {
    grepat_t *this = malloc(sizeof(grepat_t));
    this->files = vector_new();
    this->patstr = NULL;
    this->pat = NULL;
    this->count = -1;
    this->all = 0;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    grepat_t *this = _this;
    switch (c) {
    SINGLE_OPTION_ARGV('s', patstr)
    SINGLE_OPTION_CONSTANT('c', count, -1, 0)
    SINGLE_OPTION_CONSTANT('a', all, 0, 1)
    case 'f':
        vector_push(this->files, argv);
        break;
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this, char *inp, char **_out) {
    grepat_t *this = _this;
    if (this->files->size == 0)
        return (void)cmdlogrequired(&grepat, 'f');
    if (this->patstr == NULL)
        this->patstr = inp;
    if (this->patstr == NULL)
        return (void)cmdlogrequired(&grepat, 's');
    this->pat = pattern_new(this->patstr);
    if (this->pat == NULL)
        return (void)cmdlog(&grepat, "invalid pattern");
    string *out = string_using(_out);
    for (int i = 0; i < this->files->size; i++) {
        char *path = this->files->seq[i];
        FILE *file = fu_open(path, "r");
        if (file == NULL) {
            string_free(out);
            return (void)cmdlog(&grepat, "failed to open file \"%s\": %s",
                path, strerror(errno));
        }
        char *line;
        while ((line = fu_getline(file)) != NULL) {
            if (!pattern_search(this->pat, line)) {
                free(line);
                continue;
            }
            if (this->count != -1)
                this->count++;
            if (this->all) {
                if (this->count == -1)
                    fprintf(out->f, "%s\n", path);
                free(line);
                break;
            }
            if (this->count == -1)
                fprintf(out->f, "%s: %s\n", path, line);
            free(line);
        }
        fclose(file);
    }
    if (this->count != -1)
        fprintf(out->f, "%ld\n", this->count);
    fclose(out->f);
    cmdlog(&grepat, "done");
}

static void grepat_free(void *_this) {
    grepat_t *this = _this;
    vector_free(this->files);
    if (this->pat != NULL)
        pattern_free(this->pat);
    free(this);
}

const command grepat = {
    .name       = "grep",
    .optstring  = ":f:s:ca",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = grepat_free
};
