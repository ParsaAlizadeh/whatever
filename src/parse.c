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
            if (cur->size == 0)
                continue;
            if (!was_qouted && strcmp(cur->seq, PIPE) == 0) {
                vector_push(tokens, strdup(PIPENCODE));
            } else {
                vector_push(tokens, strdup(cur->seq));
            }
            string_clear(cur);
            was_qouted = 0;
            continue;
        }
        if (escaped) {
            switch (c) {
            case 'n':
                string_push(cur, '\n');
                break;
            case 't':
                string_push(cur, '\t');
                break;
            case '\n':
                break;
            case '*':
                string_push(cur, '\\');
                string_push(cur, '*');
                break;
            default:
                string_push(cur, c);
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
        string_push(cur, c);
    }
    if (escaped || qoute)
        fprintf(stderr, "scan_line: unexpected eof\n");
    if (cur->size > 0) {
        /* no pipe at the end */
        vector_push(tokens, string_free(cur));
    } else {
        free(string_free(cur));
    }
    return tokens;
}