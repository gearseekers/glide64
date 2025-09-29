#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <3dfx.h>
#include <glidesys.h>
#include "glide.h"
#include "fxglide.h"
#include "gu.h"

void
grTexDetailControl(GrTMU_t tmu, int lod_bias, FxU8 detail_max, FxU8 detail_mag)
{
}

void
grTexDownloadTablePartial(GrTexTable_t type, void *data, int start, int end)
{
}

FxU32
grTexCalcMemRequired(GrLOD_t lod, GrAspectRatio_t aspect, GrTextureFormat_t fmt)
{
    return 0;
}

FxU32
grTexMinAddress(GrTMU_t tmu)
{
    return 0;
}

FxU32
grTexMaxAddress(GrTMU_t tmu)
{
    return 0;
}

FxU32
grTexTextureMemRequired(FxU32 evenOdd, GrTexInfo *info)
{
    return 0;
}

void
grTexDownloadMipMap(GrTMU_t tmu, FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info)
{
}

void
grTexDownloadMipMapLevel(GrTMU_t tmu, FxU32 startAddress, GrLOD_t thisLod, GrLOD_t largeLod,
                         GrAspectRatio_t aspectRatio, GrTextureFormat_t format,
                         FxU32 evenOdd, void *data)
{
}