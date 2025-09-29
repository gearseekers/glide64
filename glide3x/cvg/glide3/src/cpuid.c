/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <glide.h>
#include "fxglide.h"

// Prototype for the external assembly function
extern FxU32 GetCPUIDFeatures(void);

void _grDetectProcessor(void) 
{
    FxU32 features_edx = GetCPUIDFeatures();
    
    // The x86-64 architecture guarantees SSE and SSE2 support.
    _GlideRoot.Fx64sup = 1;
    _GlideRoot.MMXsup = (features_edx & 0x00800000) != 0;
    _GlideRoot.SSEsup = 1; 
    _GlideRoot.SSE2sup = 1;
    _GlideRoot.RDTSCsup = (features_edx & 0x00000010) != 0;
    _GlideRoot.isAmdK6_3DNow = 0; // Assume false for modern x64
}