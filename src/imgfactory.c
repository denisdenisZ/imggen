#include <stdint.h>
#include <stdio.h>

#include "imgen.h"
#include "imgfactory.h"
#include "basic_patterns.h"
#include "hist_calc.h"
#include "imgio.h"

void create_image(
    uint32_t width,
    uint32_t height,
    e_bpp_t bpp,
    e_pattern_t pattern,
    const char *filename,
    void *opts)
{
    st_img_t *img = alloc_img(width, height, bpp, RGGB);

    st_hist_t *hist = {0};
    char hist_filenmae[256];
    snprintf(hist_filenmae, sizeof(hist_filenmae), "%s.hist", filename);

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

        case LINEAR_GRADIENT_PER_CH:
            set_linear_gradient_per_channel(img);
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
    hist = calc_hist(img);

    dump_raw(img, filename);
    dump_hist(hist, hist_filenmae);
}
