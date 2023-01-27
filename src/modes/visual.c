#include "visual.h"

#include <stdlib.h>
#include "movement.h"
#include "../logging.h"
#include "../editor.h"
#include "../context.h"

void visual_mode(void) {
    ed->asel = ed->acur;
    int running = 1;
    int chr;
    while (running) {
        ctx_set_edmode("VISUAL");
        editor_refresh();
        chr = getch();

        switch (chr) {
        ARROW_MOVEMENT
        VIM_MOVEMENT
        HOMEEND_MOVEMENT
        case KEY_F(1):
            running = 0;
            break;
        default:
            loginfo("undefined key: %d", chr);
        }
    }
    ed->asel = (pos_t) { -1, 0 };
}
