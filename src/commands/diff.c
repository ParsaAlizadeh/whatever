#include "diff.h"
#include "setup.h"

typedef struct {
    char *path1, *path2;
} diff_t;

static void *make(void) {
    diff_t *this = malloc(sizeof(diff_t));
    this->path1 = this->path2 = NULL;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    diff_t *this = _this;
    switch(c) {
    SINGLE_OPTION_ARGV('1', path1)
    SINGLE_OPTION_ARGV('2', path2)
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this) {
    diff_t *this = _this;
    if (this->path1 == NULL)
        return (void)cmdlogrequired(&diff, '1');
    if (this->path2 == NULL)
        return (void)cmdlogrequired(&diff, '2');
    FILE *file1 = fu_open(this->path1, "r");
    if (file1 == NULL)
        return (void)cmdlog(&diff, "failed to open file 1: %s",
            strerror(errno));
    FILE *file2 = fu_open(this->path2, "r");
    if (file2 == NULL) {
        fclose(file1);
        return (void)cmdlog(&diff, "failed to open file 2: %s",
            strerror(errno));
    }
    char *line1, *line2;
    long line_no = 0;
    while (1) {
        line_no++;
        line1 = fu_getline(file1);
        line2 = fu_getline(file2);
        if (line1 == NULL && line2 == NULL)
            break;
        if (line1 == NULL) {
            printf(">>> #%ld - $\n%s\n", line_no, line2);
            fu_copy(file2, stdout);
            free(line2);
            break;
        }
        if (line2 == NULL) {
            printf("<<< #%ld - $\n%s\n", line_no, line1);
            fu_copy(file1, stdout);
            free(line1);
            break;
        }
        if (strcmp(line1, line2) == 0) {
            printf("%s\n", line1);
        } else {
            printf("=== #%ld\n%s\n%s\n", line_no, line1, line2);
        }
        free(line1);
        free(line2);
    }
    fclose(file1);
    fclose(file2);
    cmdlog(&diff, "done");
}

const command diff = {
    .name       = "diff",
    .optstring  = ":1:2:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run,
    .free       = free
};
