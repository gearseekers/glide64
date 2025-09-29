#ifndef _FXIMG_H_
#define _FXIMG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "3dfx.h"

// Define a generic palette structure to remove the dependency on glideutl.h
typedef struct {
    FxU32 data[256];
} ImgPalette;


/* Image data formats */
typedef enum {
    IMG_FMT_NULL,
    IMG_FMT_P_8,
    IMG_FMT_YIQ,
    IMG_FMT_RLE_8,
    IMG_FMT_AYIQ,
    IMG_FMT_AP_88,
    IMG_FMT_ARGB_8888,
    IMG_FMT_RGB_565,
    IMG_FMT_ARGB_1555,
    IMG_FMT_ARGB_4444,
    IMG_FMT_RGB_332
} ImgFormat;

/* Image data structures */
typedef struct {
    FxU32   width;
    FxU32   height;
    FxU32   sizeInBytes;
    FxU32   yOrigin;       // 0 - Lower 1 - Upper
    FxU32   redBits;
    FxU32   greenBits;
    FxU32   blueBits;
    FxU32   alphaBits;
    void   *data;
} ImgInfo;

/* Function Prototypes */
FxBool imgReadFile(const char *filename, ImgInfo *info, ImgPalette *pal, void **data );

#ifdef __cplusplus
}
#endif

#endif /* !_FXIMG_H_ */