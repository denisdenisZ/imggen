
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "imgen.h"
#include "imgmath.h"
#include "hist_calc.h"

static inline uint64_t set_if_max(uint64_t cur_max, uint64_t val) {
    return val > cur_max ? val : cur_max;
}

static inline uint64_t set_if_min(uint64_t cur_min, uint64_t val) {
    return val < cur_min ? val : cur_min;
}

static inline void set_val(uint64_t *arr, size_t sz, uint64_t val) {
    for (size_t i = 0; i < sz; i++) {
        arr[i] = val;
    }
}

static inline double calc_mean(uint64_t sum, uint64_t count) {
    return (double)sum / count;
}

st_hist_t *calc_hist(st_img_t *img) {
    st_hist_t *out = malloc(sizeof(st_hist_t));
    memset(out, 0, sizeof(st_hist_t));

    const uint64_t max_val = pow2(img->bpp) - 1;
    set_val(out->min, CH_MAX, max_val);

    const size_t shift = img->bpp - log2_pow2(HIST_SIZE);

    uint64_t sum[CH_MAX] = {0};

    for (size_t y = 0; y < img->height; y++) {
        for (size_t x = 0; x < img->width; x++) {
            const uint64_t pixel = read_pixel_le(img->data + (y * img->width + x) * img->bytes_pp, img->bpp);
            const e_channel_t ch = get_channel(img->pattern, x, y);

            out->max[ch] = set_if_max(out->max[ch], pixel);
            out->min[ch] = set_if_min(out->min[ch], pixel);
            out->bins[ch][pixel >> shift]++;
            sum[ch] += pixel;
            out->count[ch]++;
        }
    }

    for (size_t ch = 0; ch < CH_MAX; ch++) {
        out->mean[ch] = out->count[ch] ? calc_mean(sum[ch], out->count[ch]) : 0;
    }

    return out;
}
