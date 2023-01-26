#include "insert.h"

#include <stdlib.h>
#include <limits.h>
#include "movement.h"
#include "../logging.h"
#include "../editor.h"
#include "../context.h"

void insert_mode(void) {
    int chr;
    while (1) {
        ctx_set_edmode("INSERT");
        editor_refresh();

        chr = getch();

        switch (chr) {
        ARROW_MOVEMENT
        HOMEEND_MOVEMENT
        case KEY_BACKSPACE:
            editor_erase();
            break;
        case KEY_F(1):
            return;
        default:
            if (chr < 0 || chr > CHAR_MAX) {
                loginfo("undefined key: %d", chr);
                break;
            }
            editor_insert(chr);
        }
    }
}
