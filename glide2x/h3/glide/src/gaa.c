#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <3dfx.h>
#include <glidesys.h>
#include "glide.h"
#include "fxglide.h"

void
grAADrawPoint(const GrVertex *e)
{
}

void
grAADrawLine(const GrVertex *v1, const GrVertex *v2)
{
}

void
grAADrawTriangle(const GrVertex *a, const GrVertex *b, const GrVertex *c,
                 FxBool ab_antialias, FxBool bc_antialias, FxBool ca_antialias)
{
}

void
grAADrawPolygon(const int nverts, const int ilist[], const GrVertex vlist[])
{
}

void
grAADrawPolygonVertexList(const int nverts, const GrVertex vlist[])
{
}