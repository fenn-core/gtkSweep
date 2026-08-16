//
// Created by pusana on 8/16/26.
//

#ifndef GTKSWEEP_UI_H
#define GTKSWEEP_UI_H

#include "error.h"


typedef enum {
    UI_PAGE_MENU,
    UI_PAGE_SETUP,
    UI_PAGE_GAME,
} ui_state_t;


game_error_t ui_init(GtkApplication *app);


#endif //GTKSWEEP_UI_H
