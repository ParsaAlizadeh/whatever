#include "parse.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <err.h>
#include "vecstr.h"

vector *scan_line(void) {
    vector *tokens = vector_new();
    string *cur = string_new();
    char c;
    int escaped = 0, qoute = 0;
    while (scanf("%c", &c) != EOF) {
        if (!escaped && !qoute && c == '\n')
            break;
        if (!escaped && !qoute && c == ' ') {
            if (cur->size == 0)
                continue;
            vector_push(tokens, strdup(cur->seq));
            string_clear(cur);
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
        string_push(cur, c);
    }
    if (escaped || qoute)
        warnx("unexpected eof");
    if (cur->size > 0) {
        vector_push(tokens, string_free(cur));
    } else {
        free(string_free(cur));
    }
    return tokens;
}