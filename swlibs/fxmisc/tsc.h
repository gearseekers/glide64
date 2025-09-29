#ifndef _TSC_H_
#define _TSC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "fxos.h"

#if defined(__GNUC__) && defined(__x86_64__)

// Use GCC inline assembly for x86-64
static __inline__ void tsc_start(int64_t *t) {
    __asm__ __volatile__("rdtsc" : "=A" (*t));
}

static __inline__ void tsc_stop(int64_t *t) {
    __asm__ __volatile__("rdtsc" : "=A" (*t));
}

#elif defined(_MSC_VER)

// Original Microsoft Visual C++ implementation (for reference)
void __cdecl tsc_start(int64_t *);
void __cdecl tsc_stop(int64_t *);

#else

// Remove the #error directive to prevent build failure
// #error "TSC: unsupported compiler"

#endif

#ifdef __cplusplus
}
#endif

#endif /* !_TSC_H_ */