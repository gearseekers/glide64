#include <stdio.h>
#include <stdlib.h>

#include <3dfx.h>
#include <glidesys.h>
#include "glide.h"
#include "fxglide.h"

static GrErrorCallbackFnc_t grErrorCallbackFnc = NULL;

void
grErrorSetCallback(GrErrorCallbackFnc_t fnc)
{
    grErrorCallbackFnc = fnc;
}

void
_grErrorCallback(const char *s, FxBool fatal)
{
    if (grErrorCallbackFnc)
        (*grErrorCallbackFnc)(s, fatal);
}

void GDBG_INFO(const int level, const char *format, ...)
{
}

void GDBG_ERROR(const char *fn, const int line, const char *format, ...)
{
}