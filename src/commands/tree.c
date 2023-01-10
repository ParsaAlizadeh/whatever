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
    string_pushes(subprefix, prefix);
    if (is_last)
        string_pushes(subprefix, "     ");
    else
        string_pushes(subprefix, "│    ");
    return string_free(subprefix);
}

static void traverse(
    const char *path,
    const char *base,
    const char *prefix,
    int depth, int maxdepth,
    int is_dir,
    int is_last,
    int show_all)
{
    printf("%s", prefix);
    if (is_last)
        printf("└────");
    else
        printf("├────");
    printf("%s\n", base);
    if (maxdepth != 0 && depth >= maxdepth)
        return;
    if (!is_dir)
        return;
    vector *items = vector_new();
    DIR *dir = opendir(path);
    if (dir == NULL)
        return (void)cmdlog(&tree, "failed to open directory: %s",
            strerror(errno));
    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(".", dp->d_name) == 0 || strcmp("..", dp->d_name) == 0)
            continue;
        if (!show_all && dp->d_name[0] == '.')
            continue;
        string *subpath = string_new();
        string_pushes(subpath, path);
        string_push(subpath, '/');
        string_pushes(subpath, dp->d_name);
        child *chd = malloc(sizeof(child));
        chd->path = string_free(subpath);
        chd->base = strdup(dp->d_name);
        chd->is_dir = dp->d_type == DT_DIR;
        vector_push(items, chd);
    }
    closedir(dir);
    if (items->size == 0)
        return;
    char *subprefix = next_prefix(prefix, is_last);
    vector_sort(items, child_cmp);
    for (int i = 0; i < items->size; i++) {
        child *cur = items->seq[i];
        int is_last = (i+1 == items->size);
        traverse(
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

static void run(void *_this) {
    tree_t *this = _this;
    traverse(".", ".", "", 0, this->level, 1, 1, this->all);
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
