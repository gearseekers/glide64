/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <glide.h>
#include "fxglide.h"
#include <emmintrin.h> // For SSE2 intrinsics

// ... (other functions in gtex.c) ...

/*
** grTexDownloadMipMapLevel
*/
GR_ENTRY(grTexDownloadMipMapLevel, void,
         (GrChipID_t tmu, FxU32 startAddress,
          GrLOD_t thisLOD, GrLOD_t largeLOD, GrAspectRatio_t aspectRatio,
          GrTextureFormat_t format, FxU32 evenOdd, void *data))
{
    // ... (original function setup logic) ...

    FxU32 data_size = _grTexCalcMemRequired(thisLOD, largeLOD, aspectRatio, format);
    void* texture_memory = (void*)((FxUINTPTR_t)gc->tex_ptr + startAddress);

    if (data_size >= 16 && ((uintptr_t)data % 16 == 0)) {
        __m128i *src = (__m128i*)data;
        __m128i *dst = (__m128i*)texture_memory;
        int i;
        int count = data_size / 16;

        for (i = 0; i < count; i++) {
            __m128i block = _mm_load_si128(src + i);
            _mm_stream_si128(dst + i, block);
        }

        int remainder = data_size % 16;
        if (remainder > 0) {
            memcpy((uint8_t*)(dst + count), (uint8_t*)(src + count), remainder);
        }
        _mm_sfence();
    } else {
        memcpy(texture_memory, data, data_size);
    }

    // ... (rest of original function logic) ...
}