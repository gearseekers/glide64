#ifndef _GU3DF_H_
#define _GU3DF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <glide.h>

typedef struct
{
  GrLOD_t           lod;
  GrAspectRatio_t   aspect_ratio;
  GrTextureFormat_t format;
  FxU8              *data;
} Gu3dfMipMap;

typedef struct
{
  int         width, height;
  int         n_mipmaps;
  Gu3dfMipMap mipmaps[GR_LOD_1 - GR_LOD_256 + 1];
} Gu3dfInfo;

FxBool gu3dfGetInfo(const char *filename, Gu3dfInfo *info);
FxBool gu3dfLoad(const char *filename, Gu3dfInfo *info);

#ifdef __cplusplus
}
#endif

#endif /* _GU3DF_H_ */