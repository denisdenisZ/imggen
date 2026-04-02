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

typedef enum { CH_R, CH_GR, CH_GB, CH_B, CH_MAX } e_channel_t;

typedef struct {
    e_channel_t tile[2][2];
} st_bayer_pattern_t;

static const st_bayer_pattern_t BAYER_RGGB = {{ {CH_R,  CH_GR}, {CH_GB, CH_B} }};
static const st_bayer_pattern_t BAYER_BGGR = {{ {CH_B,  CH_GB}, {CH_GR, CH_R} }};
static const st_bayer_pattern_t BAYER_GRBG = {{ {CH_GR, CH_R},  {CH_B,  CH_GB} }};
static const st_bayer_pattern_t BAYER_GBRG = {{ {CH_GB, CH_B},  {CH_R,  CH_GR} }};

static const st_bayer_pattern_t *bayer_lut[] = {
    [RGGB] = &BAYER_RGGB,
    [BGGR] = &BAYER_BGGR,
};

typedef struct {
    e_bpp_t bpp;
    uint8_t bytes_pp;

    uint32_t width;
    uint32_t height;

    e_bayer_pattern_t pattern;

    uint8_t *data;
} st_img_t;

typedef void (*px_transform)(uint8_t *pixel, e_bpp_t bpp, void *opt);
typedef void (*px_transform_xy)(uint8_t *pixel, e_bpp_t bpp, e_channel_t ch, uint32_t x, uint32_t y, void *opt);

void write_pixel_le(uint8_t *pixel, e_bpp_t bpp, uint64_t val);
uint64_t read_pixel_le(const uint8_t *pixel, e_bpp_t bpp);
uint64_t pow2(uint64_t num);

void transform_img(st_img_t *img, px_transform, void *opt);
void transform_img_xy(st_img_t *img, px_transform_xy, void *opt);

e_channel_t get_channel(e_bayer_pattern_t pattern, uint32_t x, uint32_t y);

st_img_t *alloc_img(uint32_t width, uint32_t height, e_bpp_t bpp, e_bayer_pattern_t pattern);

#endif
