#ifndef _GLIDETYPE_H_
#define _GLIDETYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char FxU8;
typedef signed char FxI8;
typedef unsigned short FxU16;
typedef signed short FxI16;
typedef signed int FxI32;
typedef unsigned int FxU32;
typedef int FxBool;
typedef float GrFloat;
typedef FxU32 GrColor_t;
typedef FxU8 GrAlpha_t;
typedef FxU32 GrMipMapId_t;
typedef FxU8 GrFog_t;
typedef void (*GrProc)();

typedef FxI32 GrSstPerf;
typedef FxI32 GrScreenResolution_t;
typedef FxI32 GrScreenRefresh_t;

typedef struct {
    int size;
    void *lfbPtr;
    FxU32 strideInBytes;
    int writeMode;
    int origin;
} GrLfbInfo_t;

typedef struct {
    int num_tmus;
} GrHwConfiguration;

typedef struct {
    FxU32 num_tris_processed;
    FxU32 num_tris_drawn;
    FxU32 num_tex_bytes_downloaded;
} GrSstPerfStats_t;

typedef enum {
  GR_BUFFER_FRONTBUFFER,
  GR_BUFFER_BACKBUFFER,
  GR_BUFFER_AUXBUFFER,
  GR_BUFFER_DEPTHBUFFER,
  GR_BUFFER_ALPHABUFFER,
  GR_BUFFER_TRIPLEBUFFER
} GrBuffer_t;

typedef enum {
  GR_ALPHABLEND_ZERO,
  GR_ALPHABLEND_SRC_ALPHA,
  GR_ALPHABLEND_SRC_COLOR,
  GR_ALPHABLEND_DST_ALPHA,
  GR_ALPHABLEND_DST_COLOR,
  GR_ALPHABLEND_ONE,
  GR_ALPHABLEND_ONE_MINUS_SRC_ALPHA,
  GR_ALPHABLEND_ONE_MINUS_SRC_COLOR,
  GR_ALPHABLEND_ONE_MINUS_DST_ALPHA,
  GR_ALPHABLEND_ONE_MINUS_DST_COLOR
} GrAlphaBlendFnc_t;

typedef enum {
  GR_CMP_NEVER,
  GR_CMP_LESS,
  GR_CMP_EQUAL,
  GR_CMP_LEQUAL,
  GR_CMP_GREATER,
  GR_CMP_NOTEQUAL,
  GR_CMP_GEQUAL,
  GR_CMP_ALWAYS
} GrCmpFnc_t;

typedef enum {
  GR_CHROMAKEY_DISABLE,
  GR_CHROMAKEY_ENABLE
} GrChromakeyMode_t;

typedef enum {
  GR_COMBINE_FUNCTION_ZERO,
  GR_COMBINE_FUNCTION_LOCAL,
  GR_COMBINE_FUNCTION_LOCAL_ALPHA,
  GR_COMBINE_FUNCTION_SCALE_OTHER,
  GR_COMBINE_FUNCTION_BLEND_OTHER,
  GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL,
  GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA,
  GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL,
  GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL,
  GR_COMBINE_FUNCTION_BLEND,
  GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL,
  GR_COMBINE_FUNCTION_BLEND_LOCAL,
  GR_COMBINE_FUNCTION_ADD_LOCAL,
  GR_COMBINE_FUNCTION_ADD_LOCAL_ALPHA,
  GR_COMBINE_FUNCTION_SCALE_LOCAL,
  GR_COMBINE_FUNCTION_SCALE_LOCAL_ALPHA
} GrCombineFunction_t;

typedef enum {
  GR_COMBINE_FACTOR_ZERO,
  GR_COMBINE_FACTOR_ONE,
  GR_COMBINE_FACTOR_LOCAL,
  GR_COMBINE_FACTOR_OTHER_ALPHA,
  GR_COMBINE_FACTOR_LOCAL_ALPHA,
  GR_COMBINE_FACTOR_TEXTURE_ALPHA,
  GR_COMBINE_FACTOR_DETAIL_FACTOR,
  GR_COMBINE_FACTOR_LOD_FRACTION,
  GR_COMBINE_FACTOR_ONE_MINUS_LOCAL,
  GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA,
  GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA,
  GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA,
  GR_COMBINE_FACTOR_ONE_MINUS_DETAIL_FACTOR,
  GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION
} GrCombineFactor_t;

typedef enum {
  GR_COMBINE_LOCAL_ITERATED,
  GR_COMBINE_LOCAL_CONSTANT
} GrCombineLocal_t;

typedef enum {
  GR_COMBINE_OTHER_ITERATED,
  GR_COMBINE_OTHER_TEXTURE
} GrCombineOther_t;

typedef enum {
  GR_CULL_DISABLE,
  GR_CULL_NEGATIVE,
  GR_CULL_POSITIVE
} GrCullMode_t;

typedef enum {
  GR_DEPTHBUFFER_DISABLE,
  GR_DEPTHBUFFER_ZBUFFER,
  GR_DEPTHBUFFER_WBUFFER
} GrDepthBufferMode_t;

typedef FxU32 GrEnableMode_t;

typedef enum {
  GR_DITHER_DISABLE,
  GR_DITHER_2x2,
  GR_DITHER_4x4
} GrDitherMode_t;

typedef enum {
  GR_FOG_DISABLE,
  GR_FOG_WITH_TABLE,
  GR_FOG_WITH_ITERATED_Z,
  GR_FOG_WITH_ITERATED_ALPHA
} GrFogMode_t;

typedef FxU32 GrHint_t;

typedef enum {
  GR_COLORFORMAT_ARGB,
  GR_COLORFORMAT_ABGR,
  GR_COLORFORMAT_RGBA,
  GR_COLORFORMAT_BGRA
} GrColorFormat_t;

typedef enum {
  GR_LFB_READ_ONLY,
  GR_LFB_WRITE_ONLY
} GrLock_t;

typedef enum {
  GR_LFBWRITEMODE_565,
  GR_LFBWRITEMODE_555,
  GR_LFBWRITEMODE_1555,
  GR_LFBWRITEMODE_888,
  GR_LFBWRITEMODE_8888,
  GR_LFBWRITEMODE_ANY
} GrLfbWriteMode_t;

typedef enum {
  GR_ORIGIN_UPPER_LEFT,
  GR_ORIGIN_LOWER_LEFT
} GrOriginLocation_t;

typedef enum {
  GR_LFB_SRC_FMT_565,
  GR_LFB_SRC_FMT_555,
  GR_LFB_SRC_FMT_1555,
  GR_LFB_SRC_FMT_888,
  GR_LFB_SRC_FMT_8888,
  GR_LFB_SRC_FMT_332
} GrLfbSrcFmt_t;

typedef enum {
  GR_STIPPLE_DISABLE,
  GR_STIPPLE_ENABLE
} GrStippleMode_t;

typedef FxU32 GrStipplePattern_t;

typedef enum {
  GR_LOD_256, GR_LOD_128, GR_LOD_64, GR_LOD_32,
  GR_LOD_16, GR_LOD_8, GR_LOD_4, GR_LOD_2, GR_LOD_1
} GrLOD_t;

typedef enum {
  GR_ASPECT_8x1, GR_ASPECT_4x1, GR_ASPECT_2x1, GR_ASPECT_1x1,
  GR_ASPECT_1x2, GR_ASPECT_1x4, GR_ASPECT_1x8
} GrAspectRatio_t;

typedef enum {
  GR_TEXFMT_8BIT,
  GR_TEXFMT_RGB_332 = GR_TEXFMT_8BIT,
  GR_TEXFMT_YIQ_422,
  GR_TEXFMT_ALPHA_8,
  GR_TEXFMT_INTENSITY_8,
  GR_TEXFMT_ALPHA_INTENSITY_44,
  GR_TEXFMT_P_8,
  GR_TEXFMT_RSVD1,
  GR_TEXFMT_16BIT,
  GR_TEXFMT_ARGB_8332 = GR_TEXFMT_16BIT,
  GR_TEXFMT_AYIQ_8422,
  GR_TEXFMT_RGB_565,
  GR_TEXFMT_ARGB_1555,
  GR_TEXFMT_ARGB_4444,
  GR_TEXFMT_ALPHA_INTENSITY_88,
  GR_TEXFMT_AP_88
} GrTextureFormat_t;

typedef struct GrTexInfo_s {
  GrLOD_t smallLod;
  GrLOD_t largeLod;
  GrAspectRatio_t aspectRatio;
  GrTextureFormat_t format;
  void *data;
} GrTexInfo;

typedef FxU32 GrTMU_t;

typedef enum {
  GR_TEXTURECLAMP_WRAP,
  GR_TEXTURECLAMP_CLAMP
} GrTextureClampMode_t;

typedef enum {
  GR_TEXTUREFILTER_POINT_SAMPLED,
  GR_TEXTUREFILTER_BILINEAR
} GrTextureFilterMode_t;

typedef enum {
  GR_TEX_TABLE_NCC0,
  GR_TEX_TABLE_NCC1,
  GR_TEX_TABLE_PALETTE
} GrTexTable_t;

typedef enum {
  GR_MIPMAP_DISABLE,
  GR_MIPMAP_NEAREST,
  GR_MIPMAP_NEAREST_DITHER
} GrMipMapMode_t;

typedef FxU32 GrState;
typedef FxU32 GrChipID_t;

typedef struct {
    float  sow, tow, oow;
} GrTmuVertex;

typedef struct {
    float x, y, z, w;
    float r, g, b, a;
    GrTmuVertex tmuvtx[2];
} GrVertex;

#ifdef __cplusplus
}
#endif

#endif /* _GLIDETYPE_H_ */