#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "imgen.h"

static inline uint64_t pow2(uint64_t num) {
    return (1ULL << num);
}

static void transform_img(st_img_t *img, void (*transform)(uint8_t *pixel, e_bpp_t bpp, void *opt), void *opt) {
    for (size_t i = 0; i < img->width * img->height; i++) {
        transform(img->data + i * (img->bytes_pp), img->bpp, opt);
    }
}

/* ======================================================= */
/*                  Simple transformations                 */
/* ======================================================= */

/*
* It is the users responsibility to not pass value larger than the bit depth!
*/

static inline void write_pixel_le(uint8_t *pixel, e_bpp_t bpp, uint64_t val) {
    for (size_t i = 0; i < bpp / BITS_IN_BYTE; i++) {
        pixel[i] = val & BYTE_MASK;
        val >>= BITS_IN_BYTE;
    }
}

static void set_max_le(uint8_t *pixel, e_bpp_t bpp, void *opt) {
    uint64_t max_val = pow2(bpp) - 1;

    write_pixel_le(pixel, bpp, max_val);
}

static void set_min_le(uint8_t *pixel, e_bpp_t bpp, void *opt) {
    uint64_t min_val = 0;


    write_pixel_le(pixel, bpp, min_val);
}

typedef struct {
    double current;
    double step;
} st_gradient_ctx_t;
static void step_by_le(uint8_t *pixel, e_bpp_t bpp, void *opt) {
    st_gradient_ctx_t *ctx = opt;
    uint64_t val = ctx->current;

    write_pixel_le(pixel, bpp, val);

    ctx->current += ctx->step;
}

typedef struct {
    uint32_t x;
    uint32_t y;

    uint32_t pix_per_pax_x;
    uint32_t pix_per_pax_y;

    uint32_t width;

    bool invert;
} st_checkers_ctx_t;
static void set_if_in_paxel(uint8_t *pixel, e_bpp_t bpp, void *opt) {
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

void set_max(st_img_t *img) {
    transform_img(img, set_max_le, NULL);
}

void set_min(st_img_t *img) {
    transform_img(img, set_min_le, NULL);
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

st_img_t *alloc_img(uint32_t width, uint32_t height, e_bpp_t bpp) {
    // TODO: add verification
    st_img_t *img = malloc(sizeof(st_img_t));
    img->bytes_pp = bpp/BITS_IN_BYTE;
    uint8_t *data = calloc(width*height*img->bytes_pp, sizeof(uint8_t));

    img->data = data;
    img->bpp = bpp;
    img->width = width;
    img->height = height;

    return img;
}

void dump_raw(const st_img_t *img, const char *filename) {
    FILE *f = fopen(filename, "wb");
    fwrite(img->data, 1, img->width * img->height * img->bytes_pp, f);
    fclose(f);
}

