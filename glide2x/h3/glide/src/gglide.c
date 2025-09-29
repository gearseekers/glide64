#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <3dfx.h>
#include <glidesys.h>

#include "glide.h"
#include "fxglide.h"

void
grGlideInit(void)
{
}

void
grGlideShutdown(void)
{
}

void
grGlideGetVersion(char version[80])
{
    strcpy(version, "2.4.6");
}

void
grGlideGetState(GrState *state)
{
}

void
grGlideSetState(const GrState *state)
{
}

void
grResetTriStats(void)
{
}

void
grTriStats(FxU32 *trisProcessed, FxU32 *trisDrawn)
{
}