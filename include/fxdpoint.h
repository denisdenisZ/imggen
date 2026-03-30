#ifndef FXD_H
#define FXD_H

#include <stdint.h>

typedef struct {
    uint8_t frac_bits;
    uint64_t val;
} st_ufxd_t;

st_ufxd_t to_fixed(double val, uint8_t frac_bits);
double from_fixed(const st_ufxd_t *fxd);

st_ufxd_t fxd_mul(const st_ufxd_t *a, const st_ufxd_t *b);
st_ufxd_t fxd_normalize(const st_ufxd_t *a, uint8_t frac_bits);
st_ufxd_t fxd_add(const st_ufxd_t *a, const st_ufxd_t *b);
st_ufxd_t fxd_sub(const st_ufxd_t *a, const st_ufxd_t *b); // a - b
st_ufxd_t fxd_mul_and_norm(const st_ufxd_t *a, const st_ufxd_t *b);

#endif
