//
// Created by pusana on 8/9/26.
//

#include <stdio.h>
#include "error.h"


void game_error_handler(const game_error_t error_code) {
    switch (error_code) {

        case BOARD_DIMENSIONS_INVALID:
            fprintf(stderr, "FATAL: invalid board dimensions\n");
            break;

        case MINE_COUNT_INVALID:
            fprintf(stderr, "FATAL: invalid mine count\n");
            break;

        case TIME_LIMIT_INVALID:
            fprintf(stderr, "FATAL: invalid time limit\n");
            break;

        case GAME_STATE_INVALID:
            fprintf(stderr, "ERROR: invalid game state\n");
            break;

        case ALLOCATION_ERROR:
            fprintf(stderr, "FATAL: failed to allocate memory\n");
            break;

        case NULL_POINTER_ERROR:
            fprintf(stderr, "FATAL: passed pointer is null\n");
            break;


        default: return;
    }

}
