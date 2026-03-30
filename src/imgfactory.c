#include <stdint.h>
#include <stdio.h>

#include "imgen.h"
#include "imgfactory.h"

void create_image(
    uint32_t width,
    uint32_t height,
    e_bpp_t bpp,
    e_pattern_t pattern,
    const char *filename,
    void *opts)
{
    st_img_t *img = alloc_img(width, height, bpp);

    switch (pattern) {
        case MIN:
            set_min(img);
        break;

        case MAX:
            set_max(img);
        break;

        case LINEAR_GRADIENT:
            set_linear_gradient(img);
        break;

        case CHECKERS: {
            st_checkers_opts_t *opt = opts;
            set_checkers(img, opt->num_pax_x, opt->num_pax_y, opt->invert);
        }
        break;

        default:
            perror("Uknown pattern!");
        break;
    }

    dump_raw(img, filename);
}
