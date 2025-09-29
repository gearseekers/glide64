/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <math.h>
#include <memory.h>
#include <stdio.h>

#include <glide.h>
#include "fxglide.h"
#include <emmintrin.h> // SSE2 intrinsics header

/*
** guFogTableIndexToW
*/
GR_ENTRY(guFogTableIndexToW, float, (int i))
{
    GR_BEGIN("guFogTableIndexToW", 1, 1);
    GDBG_INFO_MORE(gc->myLevel, "(%d)\n", i);
    return gc->state.fog_table[i];
}

/*
** guFogGenerateExp
*/
GR_ENTRY(guFogGenerateExp, void, (GrFog_t *fog_table, float density))
{
    GR_BEGIN("guFogGenerateExp", 1, 0);
    GDBG_INFO_MORE(gc->myLevel, "(0x%p, %f)\n", fog_table, density);

    int i;
    
    __m128 sse_density = _mm_set1_ps(-density);
    __m128 sse_scale = _mm_set1_ps(1.0f / 255.0f);
    __m128 sse_one = _mm_set1_ps(1.0f);
    __m128 sse_half = _mm_set1_ps(0.5f);

    for (i = 0; i < 256; i += 4) {
        __m128 sse_i = _mm_set_ps((float)(i+3), (float)(i+2), (float)(i+1), (float)i);
        __m128 sse_f = _mm_mul_ps(sse_i, sse_scale);
        sse_f = _mm_mul_ps(sse_f, sse_density);

        // Fast exp(x) approximation using 2nd order Taylor expansion: 1 + x + x^2/2
        __m128 sse_f2 = _mm_mul_ps(sse_f, sse_f);
        __m128 sse_exp_approx = _mm_add_ps(sse_one, _mm_add_ps(sse_f, _mm_mul_ps(sse_f2, sse_half)));

        _mm_store_ps(&fog_table[i], sse_exp_approx);
    }
}