//
// Created by pusana on 8/5/26.
//


#include <stdint.h>
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
        return NULLPTR_EXCEPTION;
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
