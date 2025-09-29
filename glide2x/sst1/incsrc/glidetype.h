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

typedef struct {
    float x, y, z, w;
} GrVertex;

#ifdef __cplusplus
}
#endif

#endif /* _GLIDETYPE_H_ */