#include "normal.h"

#include <stdlib.h>
#include "movement.h"
#include "insert.h"
#include "visual.h"
#include "../logging.h"
#include "../editor.h"
#include "../context.h"

void normal_mode(int n_cmds, const command all_cmds[]) {
    (void) n_cmds;
    (void) all_cmds;
    init_ncurses();
    int quits = 0;
    int chr;
    while (1) {
        ctx_set_edmode("NORMAL");
        editor_refresh();
        chr = getch();

        if (chr == 'q') {
            loginfo("q");
            if (++quits >= 2)
                break;
            continue;
        } else {
            quits = 0;
        }

        switch (chr) {
        ARROW_MOVEMENT
        VIM_MOVEMENT
        HOMEEND_MOVEMENT
        case 'a':
            insert_mode();
            break;
        case 'v':
            visual_mode();
            break;
        default:
            loginfo("undefined key: %d", chr);
        }
    }
    end_ncurses();
}
