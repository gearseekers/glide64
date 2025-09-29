/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
**
** x64 SSE2 Triangle Assembly
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

    // Get the global context pointer 'gc'
    mov     rax, QWORD PTR [gc]
    
    // Get the address of the state structure (assuming offset 0 from gc)
    mov     r8, [rax + 0] 
    
    // Use SSE2 to move vertex data. Use movaps for 16-byte aligned data.
    movaps  xmm0, [rdi]
    movaps  xmm1, [rsi]
    movaps  xmm2, [rdx]
    
    // Store vertex data into the gc->state structure.
    // NOTE: These offsets are illustrative and must match your GrState struct.
    // Example: triangle_vertex_a is at offset 128 in the state struct.
    movaps  [r8 + 128], xmm0
    movaps  [r8 + 144], xmm1
    movaps  [r8 + 160], xmm2

    // The C code will handle sending commands to the hardware
    // after the state is updated by this assembly function.
    
    pop     rbp
    ret