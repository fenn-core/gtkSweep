#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "game.h"


int main(void) {
    uint64_t seed;

    generate_seed(&seed);

    printf("%" PRIu64 "\n", seed);

    return 0;
}