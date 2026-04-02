#include "imgen.h"

typedef enum {
    MIN,
    MAX,
    LINEAR_GRADIENT,
    LINEAR_GRADIENT_PER_CH,
    CHECKERS,
} e_pattern_t;

void create_image(
    uint32_t width,
    uint32_t height,
    e_bpp_t bpp,
    e_pattern_t pattern,
    const char *filename,
    void *opts);
