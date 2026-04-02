#ifndef IMGIO_H
#define IMGIO_H

#include "imgen.h"
#include "hist_calc.h"

st_img_t *load_img(const char *filename, uint32_t width, uint32_t height, e_bpp_t bpp, e_bayer_pattern_t pattern);
st_hist_t *load_hist(const char *filename);

void dump_raw(const st_img_t *img, const char *filename);
void dump_hist(const st_hist_t *hist, const char *filename);

#endif
