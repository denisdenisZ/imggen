#ifndef IMGEN_H
#define IMGEN_H

#include <stdbool.h>
#include <stdint.h>

#define BITS_IN_BYTE 8U
#define BYTE_MASK 0xFF

typedef enum {
    BPP8  = 8,   // 1 byte
    BPP12 = 12,  // 2 bytes
    BPP16 = 16,  // 2 bytes
    BPP24 = 24,  // 4 bytes
    BPP26 = 26   // 4 Bytes
} e_bpp_t;

typedef enum {
    RGGB,
    BGGR
} e_bayer_pattern_t;

typedef struct {
    e_bpp_t bpp;
    uint8_t bytes_pp;
    uint32_t width;
    uint32_t height;
    uint8_t *data;
} st_img_t;

typedef struct {
    uint32_t num_pax_x;
    uint32_t num_pax_y;
    bool invert;
} st_checkers_opts_t;

void set_max(st_img_t *img);
void set_min(st_img_t *img);
void set_linear_gradient(st_img_t *img);
void set_checkers(st_img_t *img, uint8_t num_pax_x, uint8_t num_pax_y, bool invert);

st_img_t *alloc_img(uint32_t width, uint32_t height, e_bpp_t bpp);
void dump_raw(const st_img_t *img, const char *filename);

#endif
