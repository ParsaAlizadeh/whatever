#include "insertstr.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../fileutil.h"

typedef struct {
    char *path;
    char *str;
    int line_no, col_no;
} insertstr_t;

static void *make(void) {
    insertstr_t *this = malloc(sizeof(insertstr_t));
    this->path = this->str = NULL;
    this->line_no = this->col_no = -1;
    return this;
}

static int set_opt(void *_this, int c, char *argv) {
    insertstr_t *this = _this;
    switch (c) {
    case 'f':
        if (this->path != NULL)
            return CMD_REPEATED_OPTION;
        this->path = argv;
        break;
    case 's':
        if (this->str != NULL)
            return CMD_REPEATED_OPTION;
        this->str = argv;
        break;
    case 'p':
        if (this->line_no != -1)
            return CMD_REPEATED_OPTION;
        if (sscanf(argv, "%u:%u", &this->line_no, &this->col_no) < 2) {
            fprintf(stderr, "insertstr: option \"-p\" must be in this format \"%%u:%%u\"\n");
            return CMD_FAILURE;
        }
        break;
    default:
        return CMD_UNEXPECTED;
    }
    return CMD_SUCCESS;
}

static void run(void *_this) {
    insertstr_t *this = _this;
    if (this->path == NULL) {
        fprintf(stderr, "insertstr: option \"-f\" is required\n");
        return;
    }
    if (this->str == NULL) {
        fprintf(stderr, "insertstr: option \"-s\" is required\n");
        return;
    }
    if (this->line_no == -1) {
        fprintf(stderr, "insertstr: option \"-p\" is required\n");
        return;
    }

    if (fu_backup(this->path) == -1) {
        fprintf(stderr, "insertstr: backup failed: %s\n",
            strerror(errno));
        return;
    }

    FILE *source, *tmp;
    if ((source = fopen(this->path, "r")) == NULL) {
        fprintf(stderr, "insertstr: unable to open file: %s\n",
            strerror(errno));
        return;
    }
    if ((tmp = tmpfile()) == NULL) {
        fprintf(stderr, "insertstr: unable to open temp file: %s\n",
            strerror(errno));
        fclose(source);
        return;
    }

    int cur_line_no = 1, cur_col_no = 0;
    int chr = EOF;
    do {
        if (chr != EOF) {
            fputc(chr, tmp);
            cur_col_no++;
        }
        if (chr == '\n')
            cur_line_no++, cur_col_no = 0;
        if (cur_line_no == this->line_no && cur_col_no == this->col_no)
            fputs(this->str, tmp);
    } while ((chr = fgetc(source)) != EOF);
    fflush(tmp);

    fclose(source);
    if ((source = fopen(this->path, "w")) == NULL) {
        fprintf(stderr, "insertstr: unable to open file: %s\n",
            strerror(errno));
        fclose(tmp);
        return;
    }

    fseek(tmp, 0, SEEK_SET);
    while ((chr = fgetc(tmp)) != EOF)
        fputc(chr, source);

    fclose(source);
    fclose(tmp);

    fprintf(stderr, "insertstr: done\n");
    return;
}

const command insertstr = {
    .name       = "insertstr",
    .optstring  = ":f:s:p:",
    .make       = make,
    .set_opt    = set_opt,
    .run        = run
};
