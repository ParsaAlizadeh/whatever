#include "parse.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "vecstr.h"

vector *scan_line(void) {
    vector *tokens = vector_new();
    string *cur = string_new();
    char c;
    int escaped = 0, qoute = 0, was_qouted = 0;
    while (scanf("%c", &c) != EOF) {
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
        fprintf(stderr, "scan_line: unexpected eof\n");
    if (string_size(cur) > 0) {
        /* no pipe at the end */
        vector_push(tokens, string_free(cur));
    } else {
        free(string_free(cur));
    }
    return tokens;
}