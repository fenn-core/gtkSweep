//
// Created by pusana on 8/5/26.
//


#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "game.h"
#include "rng.h"
#include "error.h"


game_error_t board_init(game_board_t *board,
                        const size_t rows,
                        const size_t cols,
                        const size_t mine_count,
                        const size_t time_limit) {
    if (rows < 3 || cols < 3) {
        return BOARD_DIMENSIONS_INVALID;
    }

    if (mine_count == 0 || mine_count >= rows * cols) {
        return MINE_COUNT_INVALID;
    }

    if (time_limit <= 3) {
        return TIME_LIMIT_INVALID;
    }

    game_cell_t *temp = malloc(rows * cols * sizeof(game_cell_t));

    if (temp == NULL) {
        return ALLOCATION_ERROR;
    }

    board->cell_buffer = temp;

    size_t cell_count = rows * cols;
    for (size_t i = 0; i < cell_count; ++i) {
        board->cell_buffer[i].is_flagged = false;
        board->cell_buffer[i].is_revealed = false;
    }

    board->rows = rows;
    board->cols = cols;
    board->mine_count = mine_count;
    board->flags_placed = 0;
    board->hidden_safe_cells = cell_count - mine_count;

    board->time_limit = time_limit;
    board->time_elapsed = 0;

    board->game_state = GAME_READY;

    return GAME_ERROR_NONE;
}


game_error_t board_destroy(game_board_t *board) {
    if (board == NULL || board->cell_buffer == NULL) {
        return NULL_POINTER_ERROR;
    }
    free(board->cell_buffer);
    board->cell_buffer = NULL;

    board->rows = 0;
    board->cols = 0;
    board->mine_count = 0;
    board->flags_placed = 0;
    board->hidden_safe_cells = 0;

    board->time_limit = 0;
    board->time_elapsed = 0;

    board->game_state = GAME_END;

    return GAME_ERROR_NONE;
}


game_error_t board_place_mines(game_board_t *board, size_t cell_count, size_t mine_count) {
    if (board == NULL || board->cell_buffer == NULL) {
        return NULL_POINTER_ERROR;
    }

    for (size_t i = 0; i < cell_count; ++i) {
        board->cell_buffer[i].is_flagged = false;
        board->cell_buffer[i].is_revealed = false;
        board->cell_buffer[i].is_mine = false;
    }

    size_t mine_indexes[mine_count];
    generate_mine_indexes(mine_indexes, cell_count, mine_count);

    for (size_t j = 0; j < mine_count; ++j) {
        board->cell_buffer[mine_indexes[j]].is_mine = true;
    }

    for (size_t k = 0; k < cell_count; ++k) {
        size_t x = (k % board->cols) + 1;
        size_t y = (k / board->cols) + 1;
        int adj_mines = 0;

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0)
                    continue;

                int64_t nx = (int64_t) x + dx;
                int64_t ny = (int64_t) y + dy;

                if (nx <= 0 || ny <= 0) {
                    continue;
                }

                const size_t adj_cell_idx =
                        (nx - 1) + (ny - 1) * board->cols;

                if (board->cell_buffer[adj_cell_idx].is_mine) {
                    adj_mines += 1;
                }
            }
        }
        board->cell_buffer[k].adjacent_mines = adj_mines;
    }

    return GAME_ERROR_NONE;

}


game_error_t board_reset(game_board_t *board) {
    if (board == NULL) {
        return NULL_POINTER_ERROR;
    }

    size_t cell_count = board->rows * board->cols;

    for (size_t i = 0; i < cell_count; ++i) {
        board->cell_buffer[i].is_flagged = false;
        board->cell_buffer[i].is_revealed = false;
    }

    board->flags_placed = 0;
    board->hidden_safe_cells = cell_count - board->mine_count;
    board->time_elapsed = 0;
    board->game_state = GAME_READY;

    return GAME_ERROR_NONE;
}


game_error_t on_cell_clicked(game_board_t *board, const size_t x, const size_t y) {
    if (board == NULL || board->cell_buffer == NULL) {
        return NULL_POINTER_ERROR;
    }
    if (x > board->cols || 1 > x || y > board->rows || 1 > y) {
        return CLICK_INDEX_INVALID;
    }

    const size_t cell_idx = (x - 1) + (y - 1) * board->cols; // 1 based board indexing
    board->cell_buffer[cell_idx].is_revealed = true;

    if (board->cell_buffer[cell_idx].is_mine) {
        board->game_state = GAME_LOST;
    }

    board->hidden_safe_cells--;
    return GAME_ERROR_NONE;
}


game_error_t on_cell_right_clicked(game_board_t *board, const size_t x, const size_t y) {
    if (board == NULL || board->cell_buffer == NULL) {
        return NULL_POINTER_ERROR;
    }

    if (x > board->cols || 1 > x || y > board->rows || 1 > y) {
        return CLICK_INDEX_INVALID;
    }

    const size_t cell_idx = (x - 1) + (y - 1) * board->cols; // 1 based board indexing
    if (board->cell_buffer[cell_idx].is_revealed) {
        return GAME_ERROR_NONE;
    }

    if (board->cell_buffer[cell_idx].is_flagged) {
        board->cell_buffer[cell_idx].is_flagged = false;
    } else {
        board->cell_buffer[cell_idx].is_flagged = true;
    }

    return GAME_ERROR_NONE;
}


game_error_t on_cell_middle_clicked(game_board_t *board, const size_t x, const size_t y) {
    if (board == NULL || board->cell_buffer == NULL) {
        return NULL_POINTER_ERROR;
    }

    size_t cells_to_reveal[8] = {0};
    int reveal_idx = 0;

    const size_t cell_idx = (x - 1) + (y - 1) * board->cols;
    int adj_flags = 0;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0)
                continue;

            int64_t nx = (int64_t) x + dx;
            int64_t ny = (int64_t) y + dy;

            if (nx <= 0 || ny <= 0) {
                continue;
            }

            const size_t adj_cell_idx =
                    (nx - 1) + (ny - 1) * board->cols;

            if (board->cell_buffer[adj_cell_idx].is_flagged) {
                adj_flags += 1;
            } else {
                cells_to_reveal[reveal_idx] = adj_cell_idx;
                reveal_idx++;
            }
        }
    }

    if (board->cell_buffer[cell_idx].is_flagged ||
        board->cell_buffer[cell_idx].adjacent_mines != adj_flags) {
        return GAME_ERROR_NONE;
    }

    for (int i = 0; i <= reveal_idx; ++i) {
        board->cell_buffer[cells_to_reveal[i]].is_revealed = true;
    }

    board->hidden_safe_cells -= reveal_idx;
    return GAME_ERROR_NONE;
}
