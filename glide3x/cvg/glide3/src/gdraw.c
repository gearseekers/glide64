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
#include <emmintrin.h> // SSE2 intrinsics header

/*
** grDrawPoint
*/
GR_ENTRY(grDrawPoint, void, (const void *p))
{
  GR_BEGIN_NOFIFOCHECK("grDrawPoint", 1);
  GDBG_INFO_MORE(gc->myLevel, "(0x%p)\n", p);
  GR_FLUSH_STATE();

  _mm_store_ps((float*)&gc->cmdTransportInfo.triPacket[3], _mm_load_ps((const float*)p));
  
  gc->cmdTransportInfo.paramMask = 0;
  gc->cmdTransportInfo.module = 0;
  gc->cmdTransportInfo.sst = 1;
  gc->cmdTransportInfo.writes = 1;
  gc->cmdTransportInfo.size = 1;
  gc->cmdTransportInfo.type = 3;
  gc->cmdTransportInfo.packet[0].u = (0x0 << 24) | (1 << 12) | (GR_PARAM_XY << 8);
  
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

  _mm_store_ps((float*)&gc->cmdTransportInfo.triPacket[3], _mm_load_ps((const float*)v1));
  _mm_store_ps((float*)&gc->cmdTransportInfo.triPacket[4], _mm_load_ps((const float*)v2));

  gc->cmdTransportInfo.paramMask = 0;
  gc->cmdTransportInfo.module = 0;
  gc->cmdTransportInfo.sst = 1;
  gc->cmdTransportInfo.writes = 2;
  gc->cmdTransportInfo.size = 2;
  gc->cmdTransportInfo.type = 3;
  gc->cmdTransportInfo.packet[0].u = (0x0 << 24) | (1 << 12) | (GR_PARAM_XY << 8);
  gc->cmdTransportInfo.packet[1].u = (0x0 << 24) | (1 << 12) | ((GR_PARAM_XY + 1) << 8);
  
  GR_END();
} /* grDrawLine */

/*
** grDrawTriangle
*/
GR_ENTRY(grDrawTriangle, void,
         (const void *a, const void *b, const void *c))
{
  GR_BEGIN_NOFIFOCHECK("grDrawTriangle", 3);
  GDBG_INFO_MORE(gc->myLevel, "(0x%p, 0x%p, 0x%p)\n", a, b, c);
  GR_FLUSH_STATE();

  _mm_store_ps((float*)&gc->cmdTransportInfo.triPacket[3], _mm_load_ps((const float*)a));
  _mm_store_ps((float*)&gc->cmdTransportInfo.triPacket[4], _mm_load_ps((const float*)b));
  _mm_store_ps((float*)&gc->cmdTransportInfo.triPacket[5], _mm_load_ps((const float*)c));

  gc->cmdTransportInfo.paramMask = 0;
  gc->cmdTransportInfo.module = 0;
  gc->cmdTransportInfo.sst = 1;
  gc->cmdTransportInfo.writes = 3;
  gc->cmdTransportInfo.size = 3;
  gc->cmdTransportInfo.type = 3;
  gc->cmdTransportInfo.packet[0].u = (0x0 << 24) | (1 << 12) | (GR_PARAM_XY << 8);
  gc->cmdTransportInfo.packet[1].u = (0x0 << 24) | (1 << 12) | ((GR_PARAM_XY + 1) << 8);
  gc->cmdTransportInfo.packet[2].u = (0x0 << 24) | (1 << 12) | ((GR_PARAM_XY + 2) << 8);
  
  GR_END();
} /* grDrawTriangle */