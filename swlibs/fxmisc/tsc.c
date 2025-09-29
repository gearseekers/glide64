/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <stdint.h>
#include "tsc.h"

void FX_CSTYLE tsc_start(int64_t * t)
{
    // On x86-64, use inline assembly to get the time stamp counter.
    // 'rdtsc' places the 64-bit result into EDX:EAX.
    // The "=A" constraint tells GCC to combine EDX and EAX into a 64-bit integer.
    __asm__ __volatile__("rdtsc" : "=A" (*t));
}

void FX_CSTYLE tsc_stop(int64_t * t)
{
    // The same instruction is used to get the stop time.
    __asm__ __volatile__("rdtsc" : "=A" (*t));
}