#include <stdio.h>
#include <stdlib.h>
#include "src/context.h"
#include "src/modes/normal.h"
#include "src/modes/legacy.h"

#include "src/commands/help.h"
#include "src/commands/creatfile.h"
#include "src/commands/insertstr.h"
#include "src/commands/removestr.h"
#include "src/commands/cat.h"
#include "src/commands/undo.h"
#include "src/commands/copystr.h"
#include "src/commands/cutstr.h"
#include "src/commands/pastestr.h"
#include "src/commands/findpat.h"
#include "src/commands/replacepat.h"
#include "src/commands/grepat.h"
#include "src/commands/diff.h"
#include "src/commands/tree.h"
#include "src/commands/pretty.h"
#include "src/commands/openfile.h"
#include "src/commands/savefile.h"

int main(int argc, char *argv[]) {
    if (argc == 2)
        ctx_set(CTX_PATH, argv[1]);
    const command all_cmds[] = {
        help,
        creatfile,
        openfile,
        insertstr,
        removestr,
        cat,
        undo,
        copystr,
        cutstr,
        pastestr,
        findpat,
        replacepat,
        grepat,
        diff,
        tree,
        pretty,
        savefile
    };
    ctx_allcmds = all_cmds;
    ctx_ncmds = sizeof(all_cmds) / sizeof(command);
    if (getenv("LEGACY"))
        legacy_mode();
    else
        normal_mode();
    ctx_set(CTX_NULL, NULL);
    return EXIT_SUCCESS;
}
