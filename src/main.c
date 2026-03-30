#include <stdio.h>

#include "imgen.h"
#include "imgfactory.h"

int main(void) {
    create_image(1920, 1080, BPP16, MIN, "MIN_16bpp.raw", NULL);
    create_image(1920, 1080, BPP16, MAX, "MAX_16bpp.raw", NULL);
    create_image(1920, 1080, BPP16, LINEAR_GRADIENT, "LINEAR_GRADIENT_16bpp.raw", NULL);

    st_checkers_opts_t chck_o = {
        .num_pax_x = 16,
        .num_pax_y = 40,
        .invert = false
    };
    create_image(1920, 1080, BPP16, CHECKERS, "CHECKERS_16bpp.raw", &chck_o);

    chck_o.invert = true;
    create_image(1920, 1080, BPP16, CHECKERS, "CHECKERS_INV_16bpp.raw", &chck_o);
}
