#include "visual.h"

#include <stdlib.h>
#include "movement.h"
#include "../logging.h"
#include "../editor.h"
#include "../context.h"

static void _run_command(const char *cmd, int isdelete) {
    pos_t afrom = ed->acur, ato = ed->asel;
    if (pos_less(ato, afrom)) {
        pos_t tmp = afrom;
        afrom = ato;
        ato = tmp;
    }
    int tfrom = vc_totpos(ed->vc, afrom);
    int tto = vc_totpos(ed->vc, ato);
    int count = tto - tfrom;
    if (count == 0)
        return;
    editor_runf("%s -p %d:%d ->n %d", cmd, afrom.line+1, afrom.col, count);
    if (isdelete)
        editor_set_tcur(tfrom);
}

void visual_mode(void) {
    editor_hl_reset();
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
        MISC_MOVEMENT
        case KEY_F(1):
            running = 0;
            break;
        case 'd':
            _run_command("cut", 1);
            running = 0;
            break;
        case 'y':
            _run_command("copy", 0);
            running = 0;
            break;
        case 'r':
            _run_command("remove", 1);
            running = 0;
            break;
        default:
            loginfo("undefined key: %d", chr);
        }
    }
    ed->asel = (pos_t) { -1, 0 };
}
