/*
** Copyright (c) 1997, 1998, 1999, 2000
** 3Dfx Interactive, Inc.
** All Rights Reserved.
**
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of 3Dfx Interactive, Inc.;
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of 3Dfx Interactive, Inc.
**
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished  -
** rights reserved under the Copyright Laws of the United States.
**
**
** $Revision: 1.1.1.1 $
** $Date: 2000/02/07 16:21:42 $
**
*/

#ifdef __linux__
#define FX_ASSEMBLY
#include "../../../../swlibs/fxmisc/3dfx.h"
#include "fxglide.h"

#if ( __GNUC__ >= 2 )
#if ( __GNUC_MINOR__ >= 7 )
#define GCC_VERSION_OK
#endif
#endif

#ifdef GCC_VERSION_OK
__asm__(
"       .text\n"
"       .align 4\n"
".globl " C_PRE(FX_grGet32BitRegistry) "\n"
C_PRE(FX_grGet32BitRegistry) ":\n"
"       movl 4(%esp), %eax\n"
"       movl 8(%esp), %edx\n"
"       xorl %eax, %eax\n"
"       ret\n"
"       .align 4\n"
".globl " C_PRE(FX_set_fpu_cw) "\n"
C_PRE(FX_set_fpu_cw) ":\n"
"       movl 4(%esp), %eax\n"
"       fldcw (%eax)\n"
"       ret"
);
#else
void FX_grGet32BitRegistry() {}
void FX_set_fpu_cw() {}
#endif

#else
static int foo;
#endif