//
// Created by pusana on 8/5/26.
//


#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "game.h"


#if defined(__linux__)
#include <sys/random.h>

#elif defined(__APPLE__)
#include <stdlib.h>

#elif defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>

#endif


game_error_t generate_seed(uint64_t *seed_addr) {
    if (seed_addr == NULL) {
        return NULL_POINTER_ERROR;
    }

    uint64_t seed;

#if defined(__linux__)

    const ssize_t result = getrandom(&seed, sizeof(seed), 0);

    if (result < 0 || result != sizeof(seed)) {
        return SEED_GENERATION_ERROR;
    }

#elif defined(__APPLE__)

    arc4random_buf(&seed, sizeof(seed));

#elif defined(_WIN32)

    if (!BCRYPT_SUCCESS(
        BCryptGenRandom(
            NULL,
            (PUCHAR) & seed,
            (ULONG) sizeof(seed),
            BCRYPT_USE_SYSTEM_PREFERRED_RNG))) {
        return SEED_GENERATION_ERROR;
    }

#else

#error "ERROR: unsupported platform"

#endif

    *seed_addr = seed;

    return GAME_ERROR_NONE;
}


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

    board->rows = rows;
    board->cols = cols;
    board->mine_count = mine_count;
    board->flags_placed = 0;
    board->hidden_safe_cells = rows * cols;

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


game_error_t board_place_mines() {
}


game_error_t board_reset(game_board_t *board) {
    if (board == NULL) {
        return NULL_POINTER_ERROR;
    }
    board->flags_placed = 0;
    board->hidden_safe_cells = board->rows * board->cols;
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
    }
    else {
        board->cell_buffer[cell_idx].is_flagged = true;
    }

    return GAME_ERROR_NONE;


}







