/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <stdio.h>
#include <glide.h>
#include "fxglide.h"

// External assembly function to get CPU features
extern FxU32 _get_cpu_features();

void _grDetectProcessor(void) {
    FxU32 features = _get_cpu_features();

    // In x64, SSE and SSE2 are guaranteed to be present.
    gc->cpu_features.has_sse = FXTRUE;
    gc->cpu_features.has_sse2 = FXTRUE;
    
    // Check for other features from the returned feature set
    gc->cpu_features.has_mmx = (features & 0x00800000) != 0;
    gc->cpu_features.has_3dnow = (features & 0x80000000) != 0; // Requires extended CPUID call

    // Set a generic x86-64 CPU type
    gc->cpu_type = 6;
}