#include <stdio.h>
#include <stdint.h>
#include "game.h"


int main(void) {
    uint64_t seed;

    generate_seed(&seed);

    printf("%ld\n", seed);

    return 0;
}