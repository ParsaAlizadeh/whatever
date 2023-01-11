#include "tree.h"
#include "setup.h"

#include <dirent.h>

typedef struct {
    int level;
    int all;
} tree_t;

static void *make(void) {
    tree_t *this = malloc(sizeof(tree_t));
    this->level = 0;
    this->all = 0;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    tree_t *this = _this;
    switch (c) {
    SINGLE_OPTION_SCANF('n', tree, level, 0, "%u")
    SINGLE_OPTION_CONSTANT('a', all, 0, 1)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

typedef struct {
    char *path;
    char *base;
    int is_dir;
} child;

static int child_cmp(const void *_a, const void *_b) {
    const child *a = _a, *b = _b;
    return strcmp(a->base, b->base);
}

static char *next_prefix(const char *prefix, int is_last) {
    string *subprefix = string_new();
    fprintf(subprefix->f, "%s", prefix);
    if (is_last)
        fprintf(subprefix->f, "     ");
    else
        fprintf(subprefix->f, "│    ");
    return string_free(subprefix);
}

static void traverse(
    FILE *fout,
    const char *path,
    const char *base,
    const char *prefix,
    int depth, int maxdepth,
    int is_dir,
    int is_last,
    int show_all)
{
    fprintf(fout, "%s", prefix);
    if (is_last)
        fprintf(fout, "└────");
    else
        fprintf(fout, "├────");
    fprintf(fout, "%s\n", base);
    if (maxdepth != 0 && depth >= maxdepth)
        return;
    if (!is_dir)
        return;
    DIR *dir = opendir(path);
    if (dir == NULL)
        return (void)cmdlog(&tree, "failed to open directory: %s",
            strerror(errno));
    vector *items = vector_new();
    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(".", dp->d_name) == 0 || strcmp("..", dp->d_name) == 0)
            continue;
        if (!show_all && dp->d_name[0] == '.')
            continue;
        string *subpath = string_new();
        fprintf(subpath->f, "%s/%s", path, dp->d_name);
        child *chd = malloc(sizeof(child));
        chd->path = string_free(subpath);
        chd->base = strdup(dp->d_name);
        chd->is_dir = dp->d_type == DT_DIR;
        vector_push(items, chd);
    }
    closedir(dir);
    if (items->size == 0)
        return vector_freeall(items);
    char *subprefix = next_prefix(prefix, is_last);
    vector_sort(items, child_cmp);
    for (int i = 0; i < items->size; i++) {
        child *cur = items->seq[i];
        int is_last = (i+1 == items->size);
        traverse(
            fout,
            cur->path,
            cur->base,
            subprefix,
            depth+1, maxdepth,
            cur->is_dir,
            is_last,
            show_all);
        free(cur->path);
        free(cur->base);
    }
    vector_freeall(items);
    free(subprefix);
}

static void run(void *_this, char *inp, char **_out) {
    (void)inp;
    tree_t *this = _this;
    string *out = string_using(_out);
    traverse(out->f, ".", ".", "", 0, this->level, 1, 1, this->all);
    string_free(out);
    cmdlog(&tree, "done");
}

const command tree = {
    .name       = "tree",
    .optstring  = ":n:a",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};
