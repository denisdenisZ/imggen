#include <stdint.h>

#include "fxdpoint.h"

static inline uint64_t pow2(uint64_t num) {
    return (1ULL << num);
}

st_ufxd_t to_fixed(double val, uint8_t frac_bits) {
    st_ufxd_t out = {
        .frac_bits = frac_bits,
        .val = (uint64_t) (val * pow2(frac_bits))
    };
    return out;
}

double from_fixed(const st_ufxd_t *val) {
    return (double)val->val / pow2(val->frac_bits);
}

st_ufxd_t fxd_mul(const st_ufxd_t *a, const st_ufxd_t *b) {
    st_ufxd_t out = {
        .frac_bits = a->frac_bits*2,
        .val = a->val * b->val
    };
    return out;
}

st_ufxd_t fxd_normalize(const st_ufxd_t *a, uint8_t frac_bits) {
    st_ufxd_t out = {
        .frac_bits = frac_bits,
        .val = a->val >> (a->frac_bits - frac_bits)
    };
    return out;
}

st_ufxd_t fxd_mul_and_norm(const st_ufxd_t *a, const st_ufxd_t *b) {
    st_ufxd_t out = {
        .frac_bits = a->frac_bits,
        .val = (a->val * b->val) >> a->frac_bits
    };
    return out;
}
