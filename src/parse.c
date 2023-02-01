#include "parse.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "vecstr.h"
#include "logging.h"

vector *scan_line(FILE *inp) {
    vector *tokens = vector_new();
    string *cur = string_new();
    char c;
    int escaped = 0, qoute = 0, was_qouted = 0;
    while (fscanf(inp, "%c", &c) != EOF) {
        if (!escaped && !qoute && c == '\n')
            break;
        if (!escaped && !qoute && c == ' ') {
            if (string_size(cur) == 0)
                continue;
            char *seq = string_free(cur);
            if (!was_qouted && strcmp(seq, PIPE) == 0) {
                free(seq);
                vector_push(tokens, strdup(PIPENCODE));
            } else {
                vector_push(tokens, seq);
            }
            cur = string_new();
            was_qouted = 0;
            continue;
        }
        if (escaped) {
            switch (c) {
            case 'n':
                fprintf(cur->f, "\n");
                break;
            case 't':
                fprintf(cur->f, "\t");
                break;
            case '\n':
                break;
            case '*':
                fprintf(cur->f, "\\*");
                break;
            default:
                fprintf(cur->f, "%c", c);
                break;
            }
            escaped = 0;
            continue;
        }
        if (c == '\\') {
            escaped = 1;
            continue;
        }
        if (c == '"') {
            qoute ^= 1;
            continue;
        }
        was_qouted |= qoute;
        fprintf(cur->f, "%c", c);
    }
    if (escaped || qoute)
        loginfo("scan_line: unexpected eof");
    if (string_size(cur) > 0) {
        /* no pipe at the end */
        vector_push(tokens, string_free(cur));
    } else {
        free(string_free(cur));
    }
    return tokens;
}

vector *scan_strline(const char *inp) {
    FILE *finp = fmemopen((char *)inp, strlen(inp), "r");
    vector *tokens = scan_line(finp);
    fclose(finp);
    return tokens;
}

static void multifputs(FILE *file, int n, const char *str) {
    while (n--)
        fputs(str, file);
}

enum pretty_state {
    P_WORD,
    P_SPACE,
    P_NEWLINE
};

void prettify(FILE *file, FILE *out) {
    string *cur = string_new();
    int state = P_NEWLINE;
    int chr;
    int indent = 0, prenext = 0;
    while ((chr = fgetc(file)) != EOF) {
        switch (state) {
        case P_NEWLINE:
            if (chr == '\n') {
                if (!prenext)
                    fprintf(out, "\n");
                prenext = 0;
                break;
            }
            if (isspace(chr))
                break;
            if (chr == '}') {
                indent--;
                multifputs(out, indent, "\t");
                fprintf(out, "}\n");
                prenext = 1;
                break;
            }
            multifputs(out, indent, "\t");
            ungetc(chr, file);
            prenext = 0;
            if (chr == '{') {
                state = P_SPACE;
                break;
            }
            state = P_WORD;
            break;
        case P_WORD:
            if (chr == '{') {
                fprintf(out, " ");
                state = P_SPACE;
                ungetc(chr, file);
                break;
            }
            if (chr == '}') {
                fprintf(out, "\n");
                state = P_NEWLINE;
                ungetc(chr, file);
                break;
            }
            if (isspace(chr)) {
                state = P_SPACE;
                ungetc(chr, file);
                break;
            }
            fprintf(out, "%c", (char)chr);
            break;
        case P_SPACE:
            if (chr == '{') {
                if (string_size(cur) > 0)
                    fprintf(out, " ");
                fprintf(out, "{\n");
                free(string_free(cur));
                cur = string_new();
                indent++;
                prenext = 1;
                state = P_NEWLINE;
                break;
            }
            if (chr == '}') {
                fprintf(out, "\n");
                free(string_free(cur));
                cur = string_new();
                state = P_NEWLINE;
                ungetc(chr, file);
                break;
            }
            if (chr == '\n') {
                fprintf(out, "\n");
                free(string_free(cur));
                cur = string_new();
                state = P_NEWLINE;
                break;
            }
            if (isspace(chr)) {
                fprintf(cur->f, "%c", (char)chr);
                break;
            }
            char *seq = string_free(cur);
            fprintf(out, "%s", seq);
            free(seq);
            cur = string_new();
            state = P_WORD;
            ungetc(chr, file);
            break;
        }
    }
    free(string_free(cur));
}
