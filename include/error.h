//
// Created by pusana on 8/8/26.
//

#ifndef GTKSWEEP_ERROR_H
#define GTKSWEEP_ERROR_H


typedef enum {
    GAME_ERROR_NONE,
    BOARD_DIMENSIONS_INVALID,
    MINE_COUNT_INVALID,
    TIME_LIMIT_INVALID,
    CLICK_INDEX_INVALID,
    SEED_GENERATION_ERROR,
    ALLOCATION_ERROR,
    NULL_POINTER_ERROR,
} game_error_t;


void game_error_handler(game_error_t error_code) {
    switch (error_code) {

        case SEED_GENERATION_ERROR:



        default: return;
    }

}


#endif //GTKSWEEP_ERROR_H
