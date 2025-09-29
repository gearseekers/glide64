/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
**
** x64 CPU Feature Detection using CPUID
*/

.text
.align 16

/*
** GetCPUIDFeatures
** Returns feature flags from EDX in the EAX register.
*/
.globl GetCPUIDFeatures
GetCPUIDFeatures:
    push    rbp
    mov     rbp, rsp

    // Request standard feature information
    mov     eax, 1
    cpuid

    // Move feature flags from EDX to EAX (return register)
    mov     eax, edx

    pop     rbp
    ret