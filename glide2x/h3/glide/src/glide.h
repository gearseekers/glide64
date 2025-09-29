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
** Declarations for the Glide 2.x API
**
*/

#ifndef _GLIDE_H_
#define _GLIDE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
** -----------------------------------------------------------------------
** All include files must be guarded against multiple inclusion
** -----------------------------------------------------------------------
*/

#ifndef FX_GLIDE_H_
#define FX_GLIDE_H_
#include "../../../../swlibs/fxmisc/3dfx.h"
#include "../../../incsrc/glidetype.h"
#include "../../../incsrc/glideutl.h"
#endif

/*
** -----------------------------------------------------------------------
** Rendering Functions
** -----------------------------------------------------------------------
*/

/*
** grDrawLine
*/
FX_ENTRY void FX_CALL
grDrawLine( const GrVertex *v1, const GrVertex *v2 );

/*
** grDrawPlanarPolygon
*/
FX_ENTRY void FX_CALL
grDrawPlanarPolygon( int nverts, const int ilist[], const GrVertex vlist[] );

/*
** grDrawPlanarPolygonVertexList
*/
FX_ENTRY void FX_CALL
grDrawPlanarPolygonVertexList(int nverts, const GrVertex vlist[]);

/*
** grDrawPoint
*/
FX_ENTRY void FX_CALL
grDrawPoint( const GrVertex *pt );

/*
** grDrawPolygon
*/
FX_ENTRY void FX_CALL
grDrawPolygon( int nverts, const int ilist[], const GrVertex vlist[] );

/*
** grDrawPolygonVertexList
*/
FX_ENTRY void FX_CALL
grDrawPolygonVertexList( int nverts, const GrVertex vlist[] );

/*
** grDrawTriangle
*/
FX_ENTRY void FX_CALL
grDrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c );

/*
** grDrawTriangles
*/
FX_ENTRY void FX_CALL
grDrawTriangles( int nverts, const GrVertex vlist[] );


/*
** -----------------------------------------------------------------------
** Buffer Management
** -----------------------------------------------------------------------
*/

/*
** grBufferClear
*/
FX_ENTRY void FX_CALL
grBufferClear( GrColor_t color, GrAlpha_t alpha, FxU16 depth );

/*
** grBufferSwap
*/
FX_ENTRY void FX_CALL
grBufferSwap( int swap_interval );

/*
** grRenderBuffer
*/
FX_ENTRY void FX_CALL
grRenderBuffer( GrBuffer_t buffer );

/*
** -----------------------------------------------------------------------
** Error Management
** -----------------------------------------------------------------------
*/
typedef void (FX_CALL *GrErrorCallbackFnc_t)( const char *string, FxBool fatal );

/*
** grErrorSetCallback
*/
FX_ENTRY void FX_CALL
grErrorSetCallback( GrErrorCallbackFnc_t fnc );

/*
** -----------------------------------------------------------------------
** Feature Enable/Disable
** -----------------------------------------------------------------------
*/

/*
** grAlphaBlendFunction
*/
FX_ENTRY void FX_CALL
grAlphaBlendFunction( GrAlphaBlendFnc_t rgb_sf,   GrAlphaBlendFnc_t rgb_df,
                      GrAlphaBlendFnc_t alpha_sf, GrAlphaBlendFnc_t alpha_df );

/*
** grAlphaControlsITRGBLighting
*/
FX_ENTRY void FX_CALL
grAlphaControlsITRGBLighting( FxBool enable );

/*
** grAlphaTestFunction
*/
FX_ENTRY void FX_CALL
grAlphaTestFunction( GrCmpFnc_t fnc );

/*
** grAlphaTestReferenceValue
*/
FX_ENTRY void FX_CALL
grAlphaTestReferenceValue( GrAlpha_t value );

/*
** grChromakeyMode
*/
FX_ENTRY void FX_CALL
grChromakeyMode( GrChromakeyMode_t mode );

/*
** grChromakeyValue
*/
FX_ENTRY void FX_CALL
grChromakeyValue( GrColor_t value );

/*
** grClipWindow
*/
FX_ENTRY void FX_CALL
grClipWindow( FxU32 minx, FxU32 miny, FxU32 maxx, FxU32 maxy );

/*
** grColorCombine
*/
FX_ENTRY void FX_CALL
grColorCombine( GrCombineFunction_t fnc, GrCombineFactor_t fac,
                GrCombineLocal_t local, GrCombineOther_t other,
                FxBool invert );
/*
** grAlphaCombine
*/
FX_ENTRY void FX_CALL
grAlphaCombine( GrCombineFunction_t fnc, GrCombineFactor_t fac,
                GrCombineLocal_t local, GrCombineOther_t other,
                FxBool invert );

/*
** grColorMask
*/
FX_ENTRY void FX_CALL
grColorMask( FxBool rgb, FxBool a );

/*
** grCullMode
*/
FX_ENTRY void FX_CALL
grCullMode( GrCullMode_t mode );

/*
** grConstantColorValue
*/
FX_ENTRY void FX_CALL
grConstantColorValue( GrColor_t value );

/*
** grConstantColorValue4
*/
FX_ENTRY void FX_CALL
grConstantColorValue4(float a, float r, float g, float b);

/*
** grDepthBiasLevel
*/
FX_ENTRY void FX_CALL
grDepthBiasLevel( FxI16 level );

/*
** grDepthBufferFunction
*/
FX_ENTRY void FX_CALL
grDepthBufferFunction( GrCmpFnc_t fnc );

/*
** grDepthBufferMode
*/
FX_ENTRY void FX_CALL
grDepthBufferMode( GrDepthBufferMode_t mode );

/*
** grDepthMask
*/
FX_ENTRY void FX_CALL
grDepthMask( FxBool mask );

/*
** grDisable
*/
FX_ENTRY void FX_CALL
grDisable( GrEnableMode_t mode );

/*
** grDitherMode
*/
FX_ENTRY void FX_CALL
grDitherMode( GrDitherMode_t mode );

/*
** grEnable
*/
FX_ENTRY void FX_CALL
grEnable( GrEnableMode_t mode );

/*
** grFogColorValue
*/
FX_ENTRY void FX_CALL
grFogColorValue( GrColor_t fogcolor );

/*
** grFogMode
*/
FX_ENTRY void FX_CALL
grFogMode( GrFogMode_t mode );

/*
** grFogTable
*/
FX_ENTRY void FX_CALL
grFogTable( const GrFog_t ft[] );

/*
** grGammaCorrectionValue
*/
FX_ENTRY void FX_CALL
grGammaCorrectionValue( float value );

/*
** grSplash
*/
FX_ENTRY void FX_CALL
grSplash(float x, float y, float w, float h, FxU32 frame);

/*
** -----------------------------------------------------------------------
** Glide State Management
** -----------------------------------------------------------------------
*/

/*
** grGet
*/
FX_ENTRY void FX_CALL
grGet( FxU32 pname, FxU32 plength, FxI32 *params );

/*
** grGetString
*/
FX_ENTRY const char * FX_CALL
grGetString( FxU32 pname );

/*
** grGetProcAddress
*/
FX_ENTRY GrProc FX_CALL
grGetProcAddress(char *procName);

/*
** grHint
*/
FX_ENTRY void FX_CALL
grHint( GrHint_t hint_type, FxU32 hint_mask );

/*
** grReset
*/
FX_ENTRY void FX_CALL
grReset( void );

/*
** -----------------------------------------------------------------------
** Frame Buffer Access
** -----------------------------------------------------------------------
*/

/*
** grLfbConstantAlpha
*/
FX_ENTRY void FX_CALL
grLfbConstantAlpha( GrAlpha_t alpha );

/*
** grLfbConstantDepth
*/
FX_ENTRY void FX_CALL
grLfbConstantDepth( FxU16 depth );

/*
** grLfbWriteColorFormat
*/
FX_ENTRY void FX_CALL
grLfbWriteColorFormat( GrColorFormat_t colorFormat );

/*
** grLfbWriteColorSwizzle
*/
FX_ENTRY void FX_CALL
grLfbWriteColorSwizzle( FxBool swizzleBytes, FxBool swapWords );

/*
** grLfbLock
*/
FX_ENTRY FxBool FX_CALL
grLfbLock( GrLock_t type, GrBuffer_t buffer, GrLfbWriteMode_t writeMode,
           GrOriginLocation_t origin, FxBool pixelPipeline,
           GrLfbInfo_t *info );
/*
** grLfbUnlock
*/
FX_ENTRY FxBool FX_CALL
grLfbUnlock( GrLock_t type, GrBuffer_t buffer );

/*
** grLfbWriteRegion
*/
FX_ENTRY FxBool FX_CALL
grLfbWriteRegion( GrBuffer_t dst_buffer,
                  FxU32 dst_x, FxU32 dst_y,
                  GrLfbSrcFmt_t src_format,
                  FxU32 src_width, FxU32 src_height,
                  FxI32 src_stride,
                  const void *src_data );

/*
** grLfbReadRegion
*/
FX_ENTRY FxBool FX_CALL
grLfbReadRegion( GrBuffer_t src_buffer,
                 FxU32 src_x, FxU32 src_y,
                 FxU32 src_width, FxU32 src_height,
                 FxU32 dst_stride,
                 void *dst_data );
/*
** -----------------------------------------------------------------------
** Special FX
** -----------------------------------------------------------------------
*/

/*
** grAADrawTriangle
*/
FX_ENTRY void FX_CALL
grAADrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c,
                  FxBool ab_antialias, FxBool bc_antialias, FxBool ca_antialias);


/*
** -----------------------------------------------------------------------
** Stipple
** -----------------------------------------------------------------------
*/

/*
** grStippleMode
*/
FX_ENTRY void FX_CALL
grStippleMode( GrStippleMode_t mode );

/*
** grStipplePattern
*/
FX_ENTRY void FX_CALL
grStipplePattern( GrStipplePattern_t pattern );

/*
** -----------------------------------------------------------------------
** Texture Mapping
** -----------------------------------------------------------------------
*/

/*
** grTexCalcMemRequired
*/
FX_ENTRY FxU32 FX_CALL
grTexCalcMemRequired( GrLOD_t lod, GrAspectRatio_t aspect,
                      GrTextureFormat_t fmt );

/*
** grTexTextureMemRequired
*/
FX_ENTRY FxU32 FX_CALL
grTexTextureMemRequired( FxU32 evenOdd, GrTexInfo *info );

/*
** grTexMinAddress
*/
FX_ENTRY FxU32 FX_CALL
grTexMinAddress( GrTMU_t tmu );

/*
** grTexMaxAddress
*/
FX_ENTRY FxU32 FX_CALL
grTexMaxAddress( GrTMU_t tmu );


/*
** grTexSource
*/
FX_ENTRY void FX_CALL
grTexSource( GrTMU_t tmu,
             FxU32 startAddress,
             FxU32 evenOdd,
             GrTexInfo *info );

/*
** grTexClampMode
*/
FX_ENTRY void FX_CALL
grTexClampMode( GrTMU_t tmu,
                GrTextureClampMode_t s_clampmode,
                GrTextureClampMode_t t_clampmode );

/*
** grTexCombine
*/
FX_ENTRY void FX_CALL
grTexCombine( GrTMU_t tmu,
              GrCombineFunction_t rgb_fnc,
              GrCombineFactor_t rgb_fac,
              GrCombineFunction_t alpha_fnc,
              GrCombineFactor_t alpha_fac,
              FxBool rgb_invert,
              FxBool alpha_invert );

/*
** grTexDetailControl
*/
FX_ENTRY void FX_CALL
grTexDetailControl( GrTMU_t tmu, int lod_bias, FxU8 detail_max, FxU8 detail_mag );


/*
** grTexFilterMode
*/
FX_ENTRY void FX_CALL
grTexFilterMode( GrTMU_t tmu,
                 GrTextureFilterMode_t minfilter_mode,
                 GrTextureFilterMode_t magfilter_mode );

/*
** grTexLodBiasValue
*/
FX_ENTRY void FX_CALL
grTexLodBiasValue(GrTMU_t tmu, float value);


/*
** grTexDownloadMipMap
*/
FX_ENTRY void FX_CALL
grTexDownloadMipMap( GrTMU_t tmu, FxU32 startAddress, FxU32 evenOdd,
                     GrTexInfo *info );

/*
** grTexDownloadMipMapLevel
*/
FX_ENTRY void FX_CALL
grTexDownloadMipMapLevel( GrTMU_t tmu,
                          FxU32 startAddress,
                          GrLOD_t thisLod,
                          GrLOD_t largeLod,
                          GrAspectRatio_t aspectRatio,
                          GrTextureFormat_t format,
                          FxU32 evenOdd,
                          void *data );

/*
** grTexDownloadTable
*/
FX_ENTRY void FX_CALL
grTexDownloadTable( GrTexTable_t type, void *data );

/*
** grTexDownloadTablePartial
*/
FX_ENTRY void FX_CALL
grTexDownloadTablePartial(GrTexTable_t type,
                          void *data, int start, int end);

/*
** grTexMipMapMode
*/
FX_ENTRY void FX_CALL
grTexMipMapMode( GrTMU_t tmu, GrMipMapMode_t mode, FxBool lod_blend );

/*
** grTexMulticolorCombine
*/
FX_ENTRY void FX_CALL
grTexMulticolorCombine( GrTMU_t tmu,
                        GrCombineFunction_t fnc,
                        GrCombineFactor_t fac,
                        GrCombineLocal_t local,
                        GrCombineOther_t other,
                        FxBool invert );

/*
** grTexSource
*/
FX_ENTRY void FX_CALL
grTexSource( GrTMU_t tmu, FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info );


/*
** -----------------------------------------------------------------------
** Glide configuration and initialization
** -----------------------------------------------------------------------
*/

/*
** grSstSelect
*/
FX_ENTRY void FX_CALL
grSstSelect( int which_sst );

/*
** grGlideInit
*/
FX_ENTRY void FX_CALL
grGlideInit( void );

/*
** grGlideShutdown
*/
FX_ENTRY void FX_CALL
grGlideShutdown( void );

/*
** grGlideGetVersion
*/
FX_ENTRY void FX_CALL
grGlideGetVersion( char version[80] );

/*
** grGlideGetState
*/
FX_ENTRY void FX_CALL
grGlideGetState(GrState *state);

/*
** grGlideSetState
*/
FX_ENTRY void FX_CALL
grGlideSetState(const GrState *state);


/*
** grResetTriStats
*/
FX_ENTRY void FX_CALL
grResetTriStats( void );


/*
** grTriStats
*/
FX_ENTRY void FX_CALL
grTriStats( FxU32 *trisProcessed, FxU32 *trisDrawn );

/*
** Video
*/
FX_ENTRY FxBool FX_CALL
grUpdateSelect( int newSelect );

FX_ENTRY FxBool FX_CALL
grVoodooFile(const char *filename, const void *data, FxU32 size, FxU32 flags);

#ifdef __cplusplus
}
#endif

#endif /* _GLIDE_H_ */