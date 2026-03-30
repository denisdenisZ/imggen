#include "imgen.h"

typedef enum {
    MIN,
    MAX,
    LINEAR_GRADIENT,
    CHECKERS
} e_pattern_t;

void create_image(
    uint32_t width,
    uint32_t height,
    e_bpp_t bpp,
    e_pattern_t pattern,
    const char *filename,
    void *opts);
