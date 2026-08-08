#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "rng.h"


int main(void) {
    size_t mine_indexes[144];

    generate_mine_indexes(mine_indexes, 144, 25);

    for (size_t i = 0; i < 25; ++i) {
        printf("%" PRIu64 "\n", mine_indexes[i]);
    }

    return 0;
}