#include "insert.h"

#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include "movement.h"
#include "../logging.h"
#include "../editor.h"
#include "../context.h"

void insert_mode(void) {
    int chr;
    while (1) {
        ctx_edmode = "INSERT";
        editor_refresh();
        chr = getch();

        switch (chr) {
        NONCHAR_MOVEMENT
        case KEY_BACKSPACE:
            editor_erase();
            break;
        case KEY_F(1):
            return;
        default:
            if (isspace(chr) || isgraph(chr))
                editor_insert(chr);
            else
                loginfo("undefined key: %d", chr);
        }
    }
}
