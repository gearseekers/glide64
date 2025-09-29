#ifndef _FXHAL_H_
#define _FXHAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
** THIS FILE IS AN OS-SPECIFIC SHIM FOR THE HAL
**
** That is, this file is intended to be modified by OEMs to allow the
** HAL to run on their OS.
**
** OS-independence is achieved by having any os-specific code in this
** file, and all other HAL code is OS-independent.
*/

/*
** We need to know which hardware is being built, so that we can include
** the appropriate hardware-specific header file.
*/
#if defined(H3)
#include "h3info.h"
#elif defined(CVG)
#include "cvginfo.h"
#elif defined(SST1)
#include "sst1info.h"
#else
#error "Board type not defined"
#endif

#ifdef __cplusplus
}
#endif

#endif /* _FXHAL_H_ */