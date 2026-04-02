
#include <stdint.h>

#include "imgmath.h"

uint64_t pow2(uint64_t num) {
    return (1ULL << num);
}

uint32_t log2_pow2(uint64_t val) {
    return __builtin_ctzll(val);
}
