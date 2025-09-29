/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003
** The Glimpse project. All rights reserved.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** The views and conclusions contained in the software and documentation
** are those of the authors and should not be interpreted as representing
** official policies, either expressed or implied, of the Glimpse project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "3dfx.h"
#include "fxpci.h"

#include "../../fxmemmap/fxmemmap.h"

#if defined(__linux__) && defined(__i386__)

static FxBool rdmsr(FxU32 reg, FxU32 *hi, FxU32 *lo)
{
  int res;
  
  __asm__ __volatile__ (
    "rdmsr"
    : "=a" (*lo), "=d" (*hi)
    : "c" (reg)
  );

  return FXTRUE;
}

static FxBool wrmsr(FxU32 reg, FxU32 hi, FxU32 lo)
{
  __asm__ __volatile__ (
    "wrmsr"
    :
    : "c" (reg), "a" (lo), "d" (hi)
  );

  return FXTRUE;
}

#else

static FxBool rdmsr(FxU32 reg, FxU32 *hi, FxU32 *lo)
{
  return FXFALSE;
}

static FxBool wrmsr(FxU32 reg, FxU32 hi, FxU32 lo)
{
  return FXFALSE;
}

#endif

FxBool pciMapMSR(pciDevice_t *dev, FxU32 which, FxU32 *addr, FxU32 *len)
{
  return FXFALSE;
}

FxBool pciUnmapMSR(pciDevice_t *dev, FxU32 which, FxU32 addr)
{
  return FXFALSE;
}

FxBool pciReadMSR(pciDevice_t *dev, FxU32 reg, FxU32 *hi, FxU32 *lo)
{
  return rdmsr(reg, hi, lo);
}

FxBool pciWriteMSR(pciDevice_t *dev, FxU32 reg, FxU32 hi, FxU32 lo)
{
  return wrmsr(reg, hi, lo);
}