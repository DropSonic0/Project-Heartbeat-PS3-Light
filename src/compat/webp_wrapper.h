#ifndef WEBP_WRAPPER_H
#define WEBP_WRAPPER_H

#define SIMPLEWEBP_IMPLEMENTATION
#include "simplewebp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    JEBP_OK = 0,
    JEBP_ERROR_INVAL,
    JEBP_ERROR_INVDATA,
    JEBP_ERROR_NOSUP
} jebp_error_t;

typedef struct {
    unsigned char r, g, b, a;
} jebp_color_t;

typedef struct {
    int width;
    int height;
    unsigned char *pixels;
} jebp_image_t;

static inline const char* jebp_error_string(jebp_error_t err) { return "WebP error"; }
static inline void jebp_free_image(jebp_image_t *image) {
    if (image->pixels) free(image->pixels);
}

static inline jebp_error_t jebp_decode(jebp_image_t *image, size_t size, const void *data) {
    simplewebp *swebp = NULL;
    simplewebp_error serr = simplewebp_load_from_memory((void*)data, size, NULL, &swebp);
    if (serr != SIMPLEWEBP_NO_ERROR) return JEBP_ERROR_INVDATA;
    
    size_t w, h;
    simplewebp_get_dimensions(swebp, &w, &h);
    image->width = (int)w;
    image->height = (int)h;
    image->pixels = (unsigned char*)malloc(w * h * 4);
    
    serr = simplewebp_decode(swebp, image->pixels, NULL);
    simplewebp_unload(swebp);
    
    if (serr != SIMPLEWEBP_NO_ERROR) {
        free(image->pixels);
        return JEBP_ERROR_INVDATA;
    }
    return JEBP_OK;
}

#ifdef __cplusplus
}
#endif

#endif
