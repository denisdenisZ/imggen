#ifndef BASIC_PAT_H
#define BASIC_PAT_H

#include "imgen.h"

typedef struct {
    uint32_t num_pax_x;
    uint32_t num_pax_y;
    bool invert;
} st_checkers_opts_t;

void set_max(st_img_t *img);
void set_min(st_img_t *img);
void set_linear_gradient(st_img_t *img);
void set_checkers(st_img_t *img, uint8_t num_pax_x, uint8_t num_pax_y, bool invert);
void set_linear_gradient_per_channel(st_img_t *img);

#endif
