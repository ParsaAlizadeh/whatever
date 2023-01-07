#include "removestr.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../fileutil.h"

typedef struct {
    char *path;
    int line_no, col_no;
    int n;
    int direction;
} removestr_t;

static void *make(void) {
    removestr_t *this = malloc(sizeof(removestr_t));
    this->path = NULL;
    this->line_no = this->col_no = this->n = -1;
    this->direction = 0;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    removestr_t *this = _this;
    switch (c) {
    case 'f':
        if (this->path != NULL)
            return CMD_REPEATED_OPTION;
        this->path = argv;
        break;
    case 'p':
        if (this->line_no != -1)
            return CMD_REPEATED_OPTION;
        if (sscanf(argv, "%u:%u", &this->line_no, &this->col_no) < 2) {
            fprintf(stderr, "removestr: option \"-p\" must be in this format \"%%u:%%u\"\n");
            return CMD_FAILURE;
        }
        break;
    case 'n':
        if (this->n != -1)
            return CMD_REPEATED_OPTION;
        if (sscanf(argv, "%u", &this->n) < 1) {
            fprintf(stderr, "removestr: option \"-n\" accepts an integer\n");
            return CMD_FAILURE;
        }
        break;
    case '>':
        if (this->direction != 0)
            return CMD_REPEATED_OPTION;
        this->direction = 1;
        break;
    case '<':
        if (this->direction != 0)
            return CMD_REPEATED_OPTION;
        this->direction = -1;
        break;
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this) {
    removestr_t *this = _this;
    if (this->path == NULL) {
        fprintf(stderr, "removestr: option \"-f\" is required\n");
        return;
    }
    if (this->line_no == -1) {
        fprintf(stderr, "removestr: option \"-p\" is required\n");
        return;
    }
    if (this->n == -1) {
        fprintf(stderr, "removestr: option \"-n\" is required\n");
        return;
    }
    if (this->direction == 0) {
        fprintf(stderr, "removestr: one of \"->\" or \"-<\" is required\n");
        return;
    }

    if (fu_backup(this->path) == -1) {
        fprintf(stderr, "removestr: backup failed, ignoring: %s\n",
            strerror(errno));
    }

    FILE *file;
    if ((file = fopen(this->path, "r")) == NULL) {
        fprintf(stderr, "removestr: unable to open file: %s\n",
            strerror(errno));
        return;
    }
    int pos = fu_whereat(file, this->line_no, this->col_no);
    fclose(file);
    if (pos == -1) {
        fprintf(stderr, "removestr: not a valid position\n");
        return;
    }
    if (this->direction == -1)
        pos -= this->n - 1;
    if (pos < 0) {
        this->n += pos;
        pos = 0;
    }
    if (fu_removeat(this->path, pos, this->n) == -1) {
        fprintf(stderr, "removestr: remove failed: %s\n",
            strerror(errno));
        return;
    }

    fprintf(stderr, "removestr: done\n");
    return;
}

const command removestr = {
    .name       = "removestr",
    .optstring  = ":f:p:n:<>",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run
};
