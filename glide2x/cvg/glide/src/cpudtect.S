/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
**
** x64 CPU Feature Detection Assembly
*/

.text
.align 16

/*
** _get_cpu_features
** Returns CPU feature flags from the CPUID instruction.
** Return value is in rax.
*/
.globl _get_cpu_features
_get_cpu_features:
    push    rbp
    mov     rbp, rsp

    // Get standard feature flags
    mov     eax, 1
    cpuid
    
    // EDX contains the standard feature flags we need (MMX, SSE, SSE2)
    // Return them in EAX as per the C function's expectation.
    mov     eax, edx
    
    pop     rbp
    ret