/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <glide.h>
#include "fxglide.h"
#include <emmintrin.h> // Header for SSE2 intrinsics

/*
** grDrawPoint
*/
GR_ENTRY(grDrawPoint, void, (const void *p)) 
{
    GR_BEGIN_NOFIFOCHECK("grDrawPoint", 1);
    GDBG_INFO_MORE(gc->myLevel, "(0x%p)\n", p);
    GR_FLUSH_STATE();

    gc->state.point_vertex = *(const GrVertex *)p;
    P6FENCE;
    
    // ... (rest of the original function logic) ...
    
    GR_END();
} /* grDrawPoint */

/*
** grDrawLine
*/
GR_ENTRY(grDrawLine, void, (const void *v1, const void *v2)) 
{
    GR_BEGIN_NOFIFOCHECK("grDrawLine", 2);
    GDBG_INFO_MORE(gc->myLevel, "(0x%p, 0x%p)\n", v1, v2);
    GR_FLUSH_STATE();

    gc->state.line_vertex_a = *(const GrVertex *)v1;
    gc->state.line_vertex_b = *(const GrVertex *)v2;
    P6FENCE;

    // ... (rest of the original function logic) ...

    GR_END();
} /* grDrawLine */

/*
** grDrawTriangle
*/
GR_ENTRY(grDrawTriangle, void, (const void *a, const void *b, const void *c)) 
{
    GR_BEGIN_NOFIFOCHECK("grDrawTriangle", 3);
    GDBG_INFO_MORE(gc->myLevel, "(0x%p, 0x%p, 0x%p)\n", a, b, c);
    GR_FLUSH_STATE();

    // SSE2 Optimization: Load and store vertex data using 128-bit registers.
    __m128 sseA = _mm_load_ps((const float*)a);
    __m128 sseB = _mm_load_ps((const float*)b);
    __m128 sseC = _mm_load_ps((const float*)c);

    _mm_store_ps((float*)&(gc->state.triangle_vertex_a), sseA);
    _mm_store_ps((float*)&(gc->state.triangle_vertex_b), sseB);
    _mm_store_ps((float*)&(gc->state.triangle_vertex_c), sseC);

    P6FENCE;

    // ... (rest of the original function logic) ...
    
    GR_END();
} /* grDrawTriangle */