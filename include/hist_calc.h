#ifndef HIST_CALC_H
#define HIST_CALC_H

#include <stdint.h>

#include "imgen.h"

#define HIST_SIZE 1024ULL

typedef struct {
    uint64_t bins[CH_MAX][HIST_SIZE];
    e_bpp_t bpp;

    uint64_t min[CH_MAX];
    uint64_t max[CH_MAX];

    double mean[CH_MAX];

    uint64_t count[CH_MAX];
} st_hist_t;

st_hist_t *calc_hist(st_img_t *img);

#endif
