/*
** Copyright (c) 1995, 1996, 1997, 1998, 1999, 2000
** 3Dfx Interactive, Inc.
** All Rights Reserved.
**
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of 3Dfx Interactive, Inc.;
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of 3Dfx Interactive, Inc.
**
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished  -
** rights reserved under the Copyright Laws of the United States.
**
**
** $Revision: 1.1.1.1 $
** $Date: 2000/02/07 16:21:42 $
**
*/

#ifndef _FXGLIDE_H_
#define _FXGLIDE_H_

/*
** -----------------------------------------------------------------------
** PRE-PROCESSOR BASED CHECKS
** -----------------------------------------------------------------------
*/

#ifdef __cplusplus
extern "C" {
#endif

/*
** -----------------------------------------------------------------------
** INCLUDE FILES
** -----------------------------------------------------------------------
*/

#include <limits.h>
#include "../../../../swlibs/fxmisc/3dfx.h"
#include "glidesys.h"
#include "glide.h"
#include "gump.h"

#ifdef __WIN32__
#include <windows.h>
#endif

#if ( ( defined( __MSDOS__ ) || defined( __DOS32__ ) ) && !defined( __DJGPP__ ) )
#include <conio.h>
#endif

#if defined(__linux__) && defined(XF86_DGA)
#include <X11/Xlib.h>
#include <X11/extensions/xf86dga.h>
#endif

#if HWC_TYPE == HWC_SYSTEM
#include "fxhal.h"
#endif

#if GLIDE_MULTIPLATFORM
#include "gdebug.h"
#endif /* GLIDE_MULTIPLATFORM */


#define GLIDE_PLATFORM_U32_MAX    (0xffffffff)

/*
** -----------------------------------------------------------------------
** DEFINES
** -----------------------------------------------------------------------
*/

#define GR_NULL_CHROMA_VALUE      0x00000000
#define GR_NULL_ALPHA_VALUE       0x00
#define GR_NULL_MIPMAP_LEVEL      -1

#define GR_MAX_TEXTURE_SIZE       256
#define GR_MAX_TEXTURE_LEVELS     9

/* the size of this is getting a little out of hand */
#define GR_LOG_MAX_TEXTURE_SIZE   8
#define GR_LOD_LOG_MAX_TEXTURE_SIZE 8

#define GR_MIN_TEXTURE_SIZE     1
#define GR_MIN_TEXTURE_LEVELS   1
#define GR_LOG_MIN_TEXTURE_SIZE 0

#define MAX_NUM_SST             4
#define MAX_NUM_TMU             3
#define GR_MAX_TMU              2

#define GR_MAX_TEXTURE_RAM      4096      /* kBytes */

/* SLI device definitions */
#define GR_SLI_MASTER           0x0
#define GR_SLI_SLAVE            0x1
#define GR_SLI_DETECTION_VAL    0x69

#define GR_BITS_RGBA            0
#define GR_BITS_RGB             1

#define GR_TEXTURE_UMA_OFFSET   0x800000

#define MAX_GAMMA_TABLE_ENTRIES 256
#define MAX_SPLASH_SIZE ( 640 * 480 * 2 )


/* Make this match the files in the P6 driver. */
#define GR_PENDING_BUFFERSWAP   0x1
#define GR_PENDING_LFB_WRITE    0x2
#define GR_PENDING_CHROMAKEY    0x4
#define GR_PENDING_TEXTURE      0x8

/* Same here */
#define GR_STATUS_NONE          0x0
#define GR_STATUS_UP            0x1
#define GR_STATUS_DOWN          0x2
#define GR_STATUS_QUEUED_BUFFER 0x4
#define GR_STATUS_MORE_MEM      0x8
#define GR_STATUS_BUFFER_SWAP_PENDING 0x10

#define GR_VIDEO_UMA_OFFSET     0x400000

/*
** -----------------------------------------------------------------------
** TYPEDEFS
** -----------------------------------------------------------------------
*/
/*
** FxI16, etc are defined in 3dfx.h which is included above
*/
typedef FxI16 GrMipMapId_t;
typedef FxI32 GrContext_t;
typedef FxU32 GrColor_t;


typedef struct GrTMUConfig_s
{
  int    tmuRev;
  int    tmuRam;
} GrTMUConfig_t;

typedef struct GrVoodooConfig_s
{
  int    fbRam;
  int    fbiRev;
  int    nTexelfx;
  FxBool sliDetect;
  GrTMUConfig_t tmuConfig[GLIDE_NUM_TMU];
  FxBool        capableOfLfbBlending;
} GrVoodooConfig_t;

#define GR_DEVCAPS_APP_FULLSCREEN      ( 1L << 0 )
#define GR_DEVCAPS_APP_WINDOWED        ( 1L << 1 )
#define GR_DEVCAPS_HARDWARE_FULLSCREEN ( 1L << 2 )
#define GR_DEVCAPS_HARDWARE_WINDOWED   ( 1L << 3 )

typedef struct
{
  FxU32 size;
  FxI32 num_devices;
  struct {
    GrVoodooConfig_t VoodooConfig;
    FxU32            devcaps;
  } device_info[1];
} GrDeviceInfo;

#if GLIDE_MULTIPLATFORM
#include "gstrip.h"
#endif /* GLIDE_MULTIPLATFORM */


#ifdef GLIDE_DEBUG
typedef struct GrDebugStats_s
{
  FxU32 trianglesDrawn;
  FxU32 pointsDrawn;
  FxU32 linesDrawn;

  FxU32 aTrianglesDrawn;
  FxU32 aPointsDrawn;
  FxU32 aLinesDrawn;

  FxU32 trianglesDrawnLOG;
  FxU32 lastFrameTris;
} GrDebugStats_t;
#endif

/*
** The GrState structure
**
** This structure stores the state of the Glide library. A pointer to this
** structure is stored in the OS-specific storage and is passed to all Glide
** functions.
*/

typedef struct GrState_s
{
  int                     open;      /* Has grStart been called? */

  int                     current_sst;
  GrVoodooConfig_t        voodooConfig;

  GrMipMapId_t            current_mm;

  int                     tmu_state[GR_MAX_TMU];

  int                     fifo_wrapped;
  int                     fifo_really_wrapped;

  int                     buffer_clear_pending;
  int                     tex_source_pending;

  GrTriangleStippleMode_t stipple_mode;
  GrStipplePattern_t      stipple_pattern;

  FxU32                   param_mask;

  FxU32                   lfb_pending;
  FxU32                   swap_pending;
  FxU32                   lost_context;

  FxU32                   current_w_red;
  FxU32                   current_w_green;
  FxU32                   current_w_blue;
  FxU32                   current_w_alpha;

  int                     num_pci_boards;
  int                     num_sli_boards;

  FxU32                   lfb_color_format;
  FxU32                   lfb_color_passes;
  FxU32                   lfb_stride;

#ifdef GLIDE_DEBUG
  GrDebugStats_t stats;
#endif

  FxU16 lockType;         /* Type of lock requested for LFB */
  FxU32 lockAddress;      /* address that lfb was locked at */
  FxI32 lockStart, lockEnd;

  int bufferSwaps;
#if defined( GLIDE_DISPATCH ) && defined( GLIDE_DEBUG )
  FxU32 framesDrawn;
#endif
  FxU32 grEnableArgs[2];
  FxU32 grDisableArgs[2];
  FxU32 fogMode;
  GrColor_t fogColor;
  FxU32 chromaKeyMode;
  GrColor_t chromaKeyValue;
  FxU32 textureChromaMode;
  GrColor_t textureChromaKeyValue;
  FxU32 textureClampMode;
  FxU32 grColorCombineArgs[2];
  FxU32 grAlphaCombineArgs[2];
  FxU32 grTexCombineArgs[2][2];
  FxU32 grAlphaControlsItuColor;
  FxU32 grConstantColorValue;
  FxU32 grConstantColorValue_Saved;
  FxU32 grAlphaTestArgs[2];
  FxU32 grDepthBufferMode;
  FxU32 grDepthBufferFunction;
  FxU32 grDepthMask;
  FxU32 grRenderBuffer;
  FxU32 grLfbWriteMode;
  FxU32 grLfbBypassEnabled;
  FxU32 grDitherMode;
  FxU32 grHintArgs[2];
  FxU32 grBufferClearArgs[3];
  FxBool grSplashDisplayed;
  FxU32 colbuf_clear_color;
  FxU32 colbuf_clear_depth;
  FxBool _colbuf_clear_depth_set;
  FxU32 grColorMaskArgs[2];
  FxU32 grCullMode;
  FxU32 grErrorCallback;
  FxU32 grSstOrigin;

#if GLIDE_MULTIPLATFORM
  GrState                   *shadow;
  struct GrVertexPool       *vtxPool;
  struct GrVertexList       *vtxList;
#if GLIDE_PACKET3_TRI_STRIP
  GrStripControls           strip;
#endif /* GLIDE_PACKET3_TRI_STRIP */
#endif

} GrState;

/*
** -----------------------------------------------------------------------
** FUNCTION PROTOTYPES
** -----------------------------------------------------------------------
*/

/*
** TEXTURE MAPPING equates and function prototypes
*/
#include "gtex.h"


#ifdef FX_GLIDE_H_
/*
** Rendering Functions
*/

GR_DIENTRY(grDrawLine,              void, ( const GrVertex *v1, const GrVertex *v2 ) );
GR_DIENTRY(grDrawPlanarPolygon,     void, ( int nverts, const int ilist[], const GrVertex vlist[] ) );
GR_DIENTRY(grDrawPlanarPolygonVertexList, void, (int nverts, const GrVertex vlist[]));
GR_DIENTRY(grDrawPoint,             void, ( const GrVertex *pt ) );
GR_DIENTRY(grDrawPolygon,           void, ( int nverts, const int ilist[], const GrVertex vlist[] ) );
GR_DIENTRY(grDrawPolygonVertexList, void, ( int nverts, const GrVertex vlist[] ) );
GR_DIENTRY(grDrawTriangle,          void, ( const GrVertex *a, const GrVertex *b, const GrVertex *c ) );
GR_DIENTRY(grDrawTriangles,         void, ( int nverts, const GrVertex vlist[] ) );

/*
** Buffer Management
*/
GR_DIENTRY(grBufferClear, void, ( GrColor_t color, GrAlpha_t alpha, FxU16 depth ) );
GR_DIENTRY(grBufferSwap,  void, ( int swap_interval ) );
GR_DIENTRY(grRenderBuffer,void, ( GrBuffer_t buffer ) );

/*
** Error Management
*/
typedef void (*GrErrorCallbackFnc_t)( const char *string, FxBool fatal );
GR_DIENTRY(grErrorSetCallback, void, ( GrErrorCallbackFnc_t fnc ) );

/*
** Feature Enable/Disable
*/
GR_DIENTRY(grAlphaBlendFunction,     void, ( GrAlphaBlendFnc_t rgb_sf, GrAlphaBlendFnc_t rgb_df, GrAlphaBlendFnc_t alpha_sf, GrAlphaBlendFnc_t alpha_df ) );
GR_DIENTRY(grAlphaControlsITRGBLighting, void, ( FxBool enable ) );
GR_DIENTRY(grAlphaTestFunction,      void, ( GrCmpFnc_t fnc ) );
GR_DIENTRY(grAlphaTestReferenceValue,void, ( GrAlpha_t value ) );
GR_DIENTRY(grChromakeyMode,          void, ( GrChromakeyMode_t mode ) );
GR_DIENTRY(grChromakeyValue,         void, ( GrColor_t value ) );
GR_DIENTRY(grClipWindow,             void, ( FxU32 minx, FxU32 miny, FxU32 maxx, FxU32 maxy ) );
GR_DIENTRY(grColorCombine,           void, ( GrCombineFunction_t fnc, GrCombineFactor_t fac, GrCombineLocal_t local, GrCombineOther_t other, FxBool invert ) );
GR_DIENTRY(grAlphaCombine,           void, ( GrCombineFunction_t fnc, GrCombineFactor_t fac, GrCombineLocal_t local, GrCombineOther_t other, FxBool invert ) );
GR_DIENTRY(grColorMask,              void, ( FxBool rgb, FxBool a ) );
GR_DIENTRY(grCullMode,               void, ( GrCullMode_t mode ) );
GR_DIENTRY(grConstantColorValue,     void, ( GrColor_t value ) );
GR_DIENTRY(grConstantColorValue4,    void, (float a, float r, float g, float b));
GR_DIENTRY(grDepthBiasLevel,         void, ( FxI16 level ) );
GR_DIENTRY(grDepthBufferFunction,    void, ( GrCmpFnc_t fnc ) );
GR_DIENTRY(grDepthBufferMode,        void, ( GrDepthBufferMode_t mode ) );
GR_DIENTRY(grDepthMask,              void, ( FxBool mask ) );
GR_DIENTRY(grDisable,                void, ( GrEnableMode_t mode ) );
GR_DIENTRY(grDitherMode,             void, ( GrDitherMode_t mode ) );
GR_DIENTRY(grEnable,                 void, ( GrEnableMode_t mode ) );
GR_DIENTRY(grFogColorValue,          void, ( GrColor_t fogcolor ) );
GR_DIENTRY(grFogMode,                void, ( GrFogMode_t mode ) );
GR_DIENTRY(grFogTable,               void, ( const GrFog_t ft[] ) );
GR_DIENTRY(grGammaCorrectionValue,   void, ( float value ) );
GR_DIENTRY(grSplash,                 void, (float x, float y, float w, float h, FxU32 frame) );
GR_DIENTRY(grGet,                    void, ( FxU32 pname, FxU32 plength, FxI32 *params ) );
GR_DIENTRY(grGetString,        const char *, ( FxU32 pname ) );
GR_DIENTRY(grGetProcAddress,   GrProc, (char *procName));


GR_DIENTRY(grLfbLock,   FxBool, ( GrLock_t type, GrBuffer_t buffer, GrLfbWriteMode_t writeMode, GrOriginLocation_t origin, FxBool pixelPipeline, GrLfbInfo_t *info ) );
GR_DIENTRY(grLfbUnlock, FxBool, ( GrLock_t type, GrBuffer_t buffer ) );
GR_DIENTRY(grLfbConstantAlpha,       void, ( GrAlpha_t alpha ) );
GR_DIENTRY(grLfbConstantDepth,       void, ( FxU16 depth ) );
GR_DIENTRY(grLfbWriteColorFormat,    void, ( GrColorFormat_t colorFormat ) );
GR_DIENTRY(grLfbWriteColorSwizzle,   void, ( FxBool swizzleBytes, FxBool swapWords ) );
GR_DIENTRY(grLfbWriteRegion,         FxBool, ( GrBuffer_t dst_buffer, FxU32 dst_x, FxU32 dst_y, GrLfbSrcFmt_t src_format, FxU32 src_width, FxU32 src_height, FxI32 src_stride, const void *src_data ) );
GR_DIENTRY(grLfbReadRegion,          FxBool, ( GrBuffer_t src_buffer, FxU32 src_x, FxU32 src_y, FxU32 src_width, FxU32 src_height, FxU32 dst_stride, void *dst_data ) );

/*
** Special FX
*/
GR_DIENTRY(grAADrawTriangle,
           void,
           ( const GrVertex *a, const GrVertex *b, const GrVertex *c,
             FxBool ab_antialias, FxBool bc_antialias, FxBool ca_antialias ) );
/*
** Stipple
*/
GR_DIENTRY(grStippleMode,    void, ( GrStippleMode_t mode ) );
GR_DIENTRY(grStipplePattern, void, ( GrStipplePattern_t pattern ) );

/*
** Texture Mapping
*/
GR_DIENTRY(grTexCalcMemRequired,
           FxU32,
           ( GrLOD_t lod, GrAspectRatio_t aspect, GrTextureFormat_t fmt ) );
GR_DIENTRY(grTexTextureMemRequired, FxU32, ( FxU32 evenOdd, GrTexInfo *info ) );
GR_DIENTRY(grTexMinAddress, FxU32, ( GrTMU_t tmu ) );
GR_DIENTRY(grTexMaxAddress, FxU32, ( GrTMU_t tmu ) );

GR_DIENTRY(grTexSource, void, ( GrTMU_t tmu, FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info ) );

GR_DIENTRY(grTexClampMode,
           void,
           ( GrTMU_t tmu, GrTextureClampMode_t s_clampmode,
             GrTextureClampMode_t t_clampmode ) );

GR_DIENTRY(grTexCombine,
           void,
           ( GrTMU_t tmu, GrCombineFunction_t rgb_fnc,
             GrCombineFactor_t rgb_fac, GrCombineFunction_t alpha_fnc,
             GrCombineFactor_t alpha_fac, FxBool rgb_invert, FxBool alpha_invert ) );

GR_DIENTRY(grTexDetailControl,
           void,
           ( GrTMU_t tmu, int lod_bias, FxU8 detail_max, FxU8 detail_mag ) );

GR_DIENTRY(grTexFilterMode,
           void,
           ( GrTMU_t tmu, GrTextureFilterMode_t minfilter_mode,
             GrTextureFilterMode_t magfilter_mode ) );

GR_DIENTRY(grTexLodBiasValue, void, ( GrTMU_t tmu, float value ) );
GR_DIENTRY(grTexDownloadMipMap,
           void,
           ( GrTMU_t tmu, FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info ) );
GR_DIENTRY(grTexDownloadMipMapLevel,
           void,
           ( GrTMU_t tmu, FxU32 startAddress, GrLOD_t thisLod, GrLOD_t largeLod,
             GrAspectRatio_t aspectRatio, GrTextureFormat_t format,
             FxU32 evenOdd, void *data ) );
GR_DIENTRY(grTexDownloadTable,
           void,
           ( GrTexTable_t type, void *data ) );
GR_DIENTRY(grTexDownloadTablePartial,
           void,
           (GrTexTable_t type, void *data, int start, int end));

GR_DIENTRY(grTexMipMapMode,
           void,
           ( GrTMU_t tmu, GrMipMapMode_t mode, FxBool lod_blend ) );

GR_DIENTRY(grTexMulticolorCombine,
           void,
           ( GrTMU_t tmu, GrCombineFunction_t fnc, GrCombineFactor_t fac,
             GrCombineLocal_t local, GrCombineOther_t other, FxBool invert ) );

GR_DIENTRY(grTexSource,
           void,
           ( GrTMU_t tmu, FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info ) );


/*
** Glide configuration and initialization
*/

GR_DIENTRY(grSstSelect,      void, ( int which_sst ) );
GR_DIENTRY(grGlideInit,      void, ( void ) );
GR_DIENTRY(grGlideShutdown,  void, ( void ) );
GR_DIENTRY(grGlideGetVersion, void, ( char version[80] ) );
GR_DIENTRY(grGlideGetState,  void, ( GrState *state ) );
GR_DIENTRY(grGlideSetState,  void, ( const GrState *state ) );
GR_DIENTRY(grResetTriStats,  void, ( void ) );
GR_DIENTRY(grTriStats,       void, ( FxU32 *trisProcessed, FxU32 *trisDrawn ) );

/* Video */
GR_DIENTRY(grUpdateSelect, FxBool, ( int newSelect ) );
GR_DIENTRY(grVoodooFile, FxBool, (const char *filename, const void *data, FxU32 size, FxU32 flags) );

/*
** Glide 2.0 Inlines
*/
#include "glidein.h"

#endif /* FX_GLIDE_H_ */

#ifdef __cplusplus
}
#endif

#endif /* _FXGLIDE_H_ */