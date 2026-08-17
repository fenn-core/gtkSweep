//
// Created by pusana on 8/5/26.
//

#ifndef GTKSWEEP_GAME_H
#define GTKSWEEP_GAME_H

#include <stdbool.h>
#include "error.h"


typedef enum {
    GAME_READY,
    GAME_RUNNING,
    GAME_WON,
    GAME_LOST,
    GAME_PAUSED,
    GAME_END,
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

    size_t time_limit;
    size_t time_elapsed;

    game_state_t game_state;
} game_board_t;


game_error_t board_init(game_board_t *board,
                        size_t rows,
                        size_t cols,
                        size_t mine_count,
                        size_t time_limit);


game_error_t board_destroy(game_board_t *board);


game_error_t board_place_mines(game_board_t *board,
                               size_t cell_count,
                               size_t mine_count,
                               uint64_t *seed);


game_error_t board_reset(game_board_t *board);


game_error_t on_cell_clicked(game_board_t *board, size_t x, size_t y);


game_error_t on_cell_right_clicked(game_board_t *board, size_t x, size_t y);


game_error_t on_cell_middle_clicked(game_board_t *board, size_t x, size_t y);


#endif //GTKSWEEP_GAME_H
