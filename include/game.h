//
// Created by pusana on 8/5/26.
//

#ifndef GTKSWEEP_GAME_H
#define GTKSWEEP_GAME_H

#include <stdbool.h>


typedef enum {
    GAME_ERROR_NONE,
    NULLPTR_EXCEPTION,
    SEED_GENERATION_ERROR,
} game_error_t;


typedef enum {
    GAME_READY,
    GAME_RUNNING,
    GAME_WON,
    GAME_LOST,
    GAME_PAUSED,
} game_state_t;


typedef struct {
    bool is_mine;
    int adjacent_mines;
    bool is_revealed;
    bool is_flagged;
} game_cell_t;


typedef struct {
    game_cell_t *cell_buffer;

    size_t rows;
    size_t cols;
    size_t mine_count;
    size_t flags_placed;
    size_t hidden_safe_cells;

    game_state_t game_state;
} game_board_t;


game_error_t generate_seed(uint64_t *seed_addr);


#endif //GTKSWEEP_GAME_H
