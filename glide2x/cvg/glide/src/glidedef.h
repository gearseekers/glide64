#ifndef _GLIDEDEF_H_
#define _GLIDEDEF_H_

#define GR_DIENTRY( a, b, c ) b FX_CALL a c
#define G3_ASPECT_TRANSLATE(a) (a)
#define G3_LOD_TRANSLATE(a) (a)

#define GR_CHECK_W(a,b,c)
#define GR_CHECK_F(a,b,c)

#define GR_MIPMAPLEVELMASK_BOTH 0x3
#define GR_MIPMAPLEVELMASK_EVEN 0x1
#define GR_MIPMAPLEVELMASK_ODD  0x2

#define SST_TEXTURE_ALIGN_MASK 0x1ff
#define SST_LOD_ASPECT_SHIFT 0
#define SST_LOD_S_IS_WIDER 0
#define SST_LOD_TSPLIT 0
#define SST_LOD_ODD 0

#endif