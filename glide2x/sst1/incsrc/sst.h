#ifndef _SST_H_
#define _SST_H_

#include "ncc.h"

#ifndef _GLIDE_H_
#include <glide.h>
#endif
#ifndef _GDEBUG_H_
#include <gdebug.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef FxI32 GrSstType;
FX_ENTRY void FX_CALL grSstSelect( int );
FX_ENTRY FxBool FX_CALL grSstWinOpen( FxU32, GrScreenResolution_t, GrScreenRefresh_t, GrColorFormat_t, GrOriginLocation_t, int, int );
FX_ENTRY void FX_CALL grSstWinClose( void );
FX_ENTRY FxBool FX_CALL grSstControl( FxU32 code );
FX_ENTRY FxBool FX_CALL grSstQueryHardware( GrHwConfiguration *hwconfig );
FX_ENTRY void FX_CALL grSstPerfStats( GrSstPerfStats_t *st );
FX_ENTRY void FX_CALL grSstResetPerfStats( void );

#ifdef __cplusplus
}
#endif

#endif