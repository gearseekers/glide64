/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <stdio.h>
#include <glide.h>
#include "fxglide.h"
#include <emmintrin.h> // SSE2 intrinsics header

/*
** grDrawTriangleStrip
*/
GR_ENTRY(grDrawTriangleStrip, void, (int nverts, const void *verts))
{
    GR_BEGIN_NOFIFOCHECK("grDrawTriangleStrip", 4);
    GDBG_INFO_MORE(gc->myLevel, "(%d, 0x%p)\n", nverts, verts);
    GR_FLUSH_STATE();

    if (nverts <= 2) return;

    const float *v = (const float*)verts;
    int i;

    for (i = 0; i < (nverts - 2); i++) {
        // Load vertices for the current triangle in the strip
        __m128 sseV0 = _mm_loadu_ps(v + (i * 4));     // Vertex i
        __m128 sseV1 = _mm_loadu_ps(v + ((i+1) * 4)); // Vertex i+1
        __m128 sseV2 = _mm_loadu_ps(v + ((i+2) * 4)); // Vertex i+2

        // Store the vertices into the command transport packet.
        // The offsets (48, 64, 80) must correspond to the actual
        // memory layout of the triPacket array in the gc structure.
        _mm_store_ps((float*)((FxUINTPTR_t)&gc->cmdTransportInfo + 48), sseV0);
        _mm_store_ps((float*)((FxUINTPTR_t)&gc->cmdTransportInfo + 64), sseV1);
        _mm_store_ps((float*)((FxUINTPTR_t)&gc->cmdTransportInfo + 80), sseV2);

        // ... (original fifo/command dispatch logic for a single triangle) ...
    }
    
    GR_END();
}