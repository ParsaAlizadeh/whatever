#include <stdio.h>
#include <stdlib.h>
#include "vecstr.h"

int main() {
    string *s = string_new();
    for (char c = 'a'; c <= 'z'; c++)
        string_push(s, c);
    printf("%s\n", s->seq);
    free(string_free(s));
    return EXIT_SUCCESS;
}