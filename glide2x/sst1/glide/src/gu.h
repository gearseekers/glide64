#ifndef _GU_H_
#define _GU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gu3df.h"

typedef struct {
    int y, i, q;
} GuNccTable;

typedef struct {
    FxU32 palette[256];
} GuTexPalette;

#ifdef __cplusplus
}
#endif

#endif /* _GU_H_ */