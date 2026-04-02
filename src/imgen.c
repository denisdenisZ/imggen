#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "imgen.h"

void transform_img(st_img_t *img, px_transform transform, void *opt) {
    for (size_t i = 0; i < img->width * img->height; i++) {
        transform(img->data + i * (img->bytes_pp), img->bpp, opt);
    }
}

void transform_img_xy(st_img_t *img, px_transform_xy transform, void *opt) {
    for (size_t y = 0; y < img->height; y++) {
        for (size_t x = 0; x < img->width; x++) {
            uint8_t *pixel = img->data + (y * img->width + x) * img->bytes_pp;
            e_channel_t ch = get_channel(img->pattern, x, y);
            transform(pixel, img->bpp, ch, x, y, opt);
        }
    }
}
/*
* It is the users responsibility to not pass value larger than the bit depth!
*/
void write_pixel_le(uint8_t *pixel, e_bpp_t bpp, uint64_t val) {
    for (size_t i = 0; i < bpp / BITS_IN_BYTE; i++) {
        pixel[i] = val & BYTE_MASK;
        val >>= BITS_IN_BYTE;
    }
}

uint64_t read_pixel_le(const uint8_t *pixel, e_bpp_t bpp) {
    uint64_t val = 0;
    for (size_t i = 0; i < bpp / BITS_IN_BYTE; i++) {
        val |= (uint64_t)pixel[i] << (i * BITS_IN_BYTE);
    }
    return val;
}

e_channel_t get_channel(e_bayer_pattern_t pattern, uint32_t x, uint32_t y) {
    return bayer_lut[pattern]->tile[y & 1][x & 1];
}

st_img_t *alloc_img(uint32_t width, uint32_t height, e_bpp_t bpp, e_bayer_pattern_t pattern) {
    // TODO: add verification
    st_img_t *img = malloc(sizeof(st_img_t));
    img->bytes_pp = bpp/BITS_IN_BYTE;

    uint8_t *data = calloc(width*height*img->bytes_pp, sizeof(uint8_t));

    img->data = data;
    img->bpp = bpp;
    img->width = width;
    img->height = height;
    img->pattern = pattern;

    return img;
}


