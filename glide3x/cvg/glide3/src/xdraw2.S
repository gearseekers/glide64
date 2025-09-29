/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
**
** x64 SSE2 Assembly for Glide3x Drawing Primitives
*/

.text
.align 16

/*
** _grDrawTriangle_x64_SSE2
**
** Arguments (System V AMD64 ABI):
** rdi: const GrVertex *a
** rsi: const GrVertex *b
** rdx: const GrVertex *c
*/
.globl _grDrawTriangle_x64_SSE2
_grDrawTriangle_x64_SSE2:
    push    rbp
    mov     rbp, rsp

    mov     rax, QWORD PTR [gc]
    
    // Get address of cmdTransportInfo (assuming offset 8 in gc struct)
    mov     r8, [rax + 8] 
    
    // Use movaps for aligned data; movups for unaligned
    movaps  xmm0, [rdi]     // Load vertex a
    movaps  xmm1, [rsi]     // Load vertex b
    movaps  xmm2, [rdx]     // Load vertex c
    
    // Store vertices into the command packet buffer
    // Offsets must match the GrCmdTransportInfo struct
    movaps  [r8 + 48], xmm0  // triPacket[3]
    movaps  [r8 + 64], xmm1  // triPacket[4]
    movaps  [r8 + 80], xmm2  // triPacket[5]
    
    // Set other members of cmdTransportInfo
    mov     DWORD PTR [r8 + 16], 0  // paramMask
    mov     DWORD PTR [r8 + 20], 0  // module
    mov     DWORD PTR [r8 + 24], 1  // sst
    mov     DWORD PTR [r8 + 28], 3  // writes
    mov     DWORD PTR [r8 + 32], 3  // size
    mov     DWORD PTR [r8 + 36], 3  // type
    
    // Setup packet command data
    mov     DWORD PTR [r8 + 96], 0x010c0000 | (GR_PARAM_XY << 8)
    mov     DWORD PTR [r8 + 100], 0x010c0000 | ((GR_PARAM_XY + 1) << 8)
    mov     DWORD PTR [r8 + 104], 0x010c0000 | ((GR_PARAM_XY + 2) << 8)
    
    pop     rbp
    ret