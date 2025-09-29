/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <stdint.h>
#include "fx64.h"

//
// Convert a 32-bit integer to a 64-bit integer
//
void _FxI32ToI64(int64_t *dst, int32_t src)
{
    *dst = (int64_t)src;
}

//
// Convert a 64-bit integer to a 32-bit integer
//
void _FxI64ToI32(int32_t *dst, const int64_t *src)
{
    *dst = (int32_t)*src;
}

//
// Add two 64-bit integers: r = a + b
//
void _FxAddI64(int64_t *r, const int64_t *a, const int64_t *b)
{
    *r = *a + *b;
}

//
// Unsigned multiply of two 32-bit integers to produce a 64-bit result
//
void _FxU32Mul(uint64_t *r, uint32_t a, uint32_t b)
{
    *r = (uint64_t)a * (uint64_t)b;
}