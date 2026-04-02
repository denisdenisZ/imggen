#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imgio.h"
#include "hist_calc.h"
#include "imgen.h"

st_img_t *load_img(const char *filename, uint32_t width, uint32_t height, e_bpp_t bpp, e_bayer_pattern_t pattern) {
    st_img_t *img = alloc_img(width, height, bpp, pattern);
    FILE *f = fopen(filename, "rb");
    fread(img->data, img->bytes_pp, width * height, f);
    fclose(f);
    return img;
}

st_hist_t *load_hist(const char *filename) {
    st_hist_t *hist = malloc(sizeof(st_hist_t));
    memset(hist, 0, sizeof(st_hist_t));
    FILE *f = fopen(filename, "r");
    char line[256];
    fgets(line, sizeof(line), f); // skip stats header
    for (size_t ch = 0; ch < CH_MAX; ch++) {
        size_t idx;
        fscanf(f, "%zu,%lu,%lu,%lf,%lu\n", &idx,
            &hist->min[ch], &hist->max[ch],
            &hist->mean[ch], &hist->count[ch]);
    }
    fgets(line, sizeof(line), f); // skip bins header
    for (size_t i = 0; i < HIST_SIZE; i++) {
        size_t bin;
        fscanf(f, "%zu,%lu,%lu,%lu,%lu\n", &bin,
            &hist->bins[CH_R][i],
            &hist->bins[CH_GR][i],
            &hist->bins[CH_GB][i],
            &hist->bins[CH_B][i]);
    }
    fclose(f);
    return hist;
}

void dump_hist(const st_hist_t *hist, const char *filename) {
    FILE *f = fopen(filename, "w");
    // stats header
    fprintf(f, "channel,min,max,mean,count\n");
    for (size_t ch = 0; ch < CH_MAX; ch++) {
        fprintf(f, "%zu,%lu,%lu,%f,%lu\n", ch,
            hist->min[ch], hist->max[ch],
            hist->mean[ch], hist->count[ch]);
    }
    // bins
    fprintf(f, "bin,R,Gr,Gb,B\n");
    for (size_t i = 0; i < HIST_SIZE; i++) {
        fprintf(f, "%zu,%lu,%lu,%lu,%lu\n", i,
            hist->bins[CH_R][i],
            hist->bins[CH_GR][i],
            hist->bins[CH_GB][i],
            hist->bins[CH_B][i]);
    }
    fclose(f);
}

void dump_raw(const st_img_t *img, const char *filename) {
    FILE *f = fopen(filename, "wb");
    fwrite(img->data, 1, img->width * img->height * img->bytes_pp, f);
    fclose(f);
}
