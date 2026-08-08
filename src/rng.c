//
// Created by pusana on 8/8/26.
//


#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "error.h"


#if defined(__linux__)
#include <sys/random.h>

#elif defined(__APPLE__)
#include <stdlib.h>

#elif defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>

#endif


typedef struct {
    uint64_t state;
    uint64_t inc;
} pcg32_random_t;

static uint32_t pcg32_random_r(pcg32_random_t *rng) {
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc | 1);
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}


static uint32_t pcg32_bounded_r(pcg32_random_t *rng, uint32_t bound) {
    uint32_t threshold = -bound % bound;

    while (1) {
        uint32_t r = pcg32_random_r(rng);
        if (r >= threshold)
            return r % bound;
    }
}


static game_error_t generate_seed(uint64_t *seed_addr) {
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


void generate_mine_indexes(size_t *mine_indexes, size_t cell_count, size_t mine_count) {
    size_t *grid = malloc(sizeof(*mine_indexes) * cell_count);
    if (grid == NULL) {
        game_error_handler(NULL_POINTER_ERROR);
    }

    for (size_t i = 0; i < cell_count; ++i) {
        grid[i] = i;
    }

    uint64_t seed;
    pcg32_random_t rng;

    game_error_handler(generate_seed(&seed));
    rng.inc = 1;
    rng.state = seed;

    for (size_t iters = 0; iters < mine_count; ++iters) {
        size_t mine_index = iters + pcg32_bounded_r(&rng, cell_count - iters);
        size_t temp = grid[mine_index];
        grid[mine_index] = grid[iters];
        grid[iters] = temp;
    }

    for (size_t j = 0; j < mine_count; ++j) {
        mine_indexes[j] = grid[j];
    }

    free(grid);

}
