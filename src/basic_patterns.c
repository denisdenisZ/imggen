#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "imgen.h"
#include "basic_patterns.h"

// =============================================================================
//
// =============================================================================

static inline void set_max_le(uint8_t *pixel, e_bpp_t bpp, void *opt) {
    uint64_t max_val = pow2(bpp) - 1;

    write_pixel_le(pixel, bpp, max_val);
}

static inline void set_min_le(uint8_t *pixel, e_bpp_t bpp, void *opt) {
    uint64_t min_val = 0;

    write_pixel_le(pixel, bpp, min_val);
}

void set_max(st_img_t *img) {
    transform_img(img, set_max_le, NULL);
}

void set_min(st_img_t *img) {
    transform_img(img, set_min_le, NULL);
}

// =============================================================================
//
// =============================================================================

typedef struct {
    double current;
    double step;
} st_gradient_ctx_t;
static inline void step_by_le(uint8_t *pixel, e_bpp_t bpp, void *opt) {
    st_gradient_ctx_t *ctx = opt;
    uint64_t val = ctx->current;

    write_pixel_le(pixel, bpp, val);

    ctx->current += ctx->step;
}

static inline double calc_linear_step(uint32_t width, uint32_t height, e_bpp_t bpp) {
    return (double)(pow2(bpp) - 1) / (width * height);
}
void set_linear_gradient(st_img_t *img) {
    st_gradient_ctx_t ctx = {
        .current = 0.0,
        .step = calc_linear_step(img->width, img->height, img->bpp)
    };
    transform_img(img, step_by_le, &ctx);
}

// =============================================================================
//
// =============================================================================

typedef struct {
    uint64_t count[CH_MAX];
    uint64_t total[CH_MAX];
    uint64_t max_val;
} st_ch_gradient_ctx_t;
static inline void ch_gradient(uint8_t *pixel, e_bpp_t bpp, e_channel_t ch, uint32_t x, uint32_t y, void *opt) {
    st_ch_gradient_ctx_t *ctx = opt;
    uint64_t val = (ctx->count[ch] * ctx->max_val) / ctx->total[ch];
    write_pixel_le(pixel, bpp, val);
    ctx->count[ch]++;
}

void set_linear_gradient_per_channel(st_img_t *img) {
    uint64_t pixels_per_ch = (img->width * img->height) / CH_MAX;
    st_ch_gradient_ctx_t ctx = {
        .count = {0},
        .total = {pixels_per_ch, pixels_per_ch, pixels_per_ch, pixels_per_ch},
        .max_val = pow2(img->bpp) - 1
    };
    transform_img_xy(img, ch_gradient, &ctx);
}

// =============================================================================
//
// =============================================================================

typedef struct {
    uint32_t x;
    uint32_t y;

    uint32_t pix_per_pax_x;
    uint32_t pix_per_pax_y;

    uint32_t width;

    bool invert;
} st_checkers_ctx_t;
static inline void set_if_in_paxel(uint8_t *pixel, e_bpp_t bpp, void *opt) {
    st_checkers_ctx_t *ctx = opt;
    uint64_t val = 0;

    if ((ctx->x / ctx->pix_per_pax_x + ctx->y / ctx->pix_per_pax_y) % 2 == ctx->invert) {
        val = pow2(bpp) - 1;
    }

    write_pixel_le(pixel, bpp, val);

    ctx->x++;
    if (ctx->x >= ctx->width) {
        ctx->x = 0;
        ctx->y++;
    }
}

void set_checkers(st_img_t *img, uint8_t num_pax_x, uint8_t num_pax_y, bool invert) {
    if (img->height % num_pax_y !=0 || img->width % num_pax_x != 0) {
        perror("provided resolution does not divide by provided paxels");
        exit(1);
    }
    uint32_t pix_per_pax_x = img->width / num_pax_x;
    uint32_t pix_per_pax_y = img->height / num_pax_y;
    st_checkers_ctx_t ctx = {
        .x = 0,
        .y = 0,
        .pix_per_pax_x = pix_per_pax_x,
        .pix_per_pax_y = pix_per_pax_y,
        .width = img->width,
        .invert = invert
    };

    transform_img(img, set_if_in_paxel, &ctx);
}
