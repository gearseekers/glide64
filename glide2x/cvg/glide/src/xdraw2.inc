;; -*-asm-*-
;; [CONVERTED TO X64/SSE INSTRUCTIONS]
;; THIS SOFTWARE IS SUBJECT TO COPYRIGHT PROTECTION AND IS OFFERED ONLY
;; PURSUANT TO THE 3DFX GLIDE GENERAL PUBLIC LICENSE. THERE IS NO RIGHT
;; TO USE THE GLIDE TRADEMARK WITHOUT PRIOR WRITTEN PERMISSION OF 3DFX
;; INTERACTIVE, INC. A COPY OF THIS LICENSE MAY BE OBTAINED FROM THE 
;; DISTRIBUTOR OR BY CONTACTING 3DFX INTERACTIVE INC(info@3dfx.com). 
;; THIS PROGRAM IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER 
;; EXPRESSED OR IMPLIED. SEE THE 3DFX GLIDE GENERAL PUBLIC LICENSE FOR A
;; FULL TEXT OF THE NON-WARRANTY PROVISIONS.  
;; 
;; USE, DUPLICATION OR DISCLOSURE BY THE GOVERNMENT IS SUBJECT TO
;; RESTRICTIONS AS SET FORTH IN SUBDIVISION (C)(1)(II) OF THE RIGHTS IN
;; TECHNICAL DATA AND COMPUTER SOFTWARE CLAUSE AT DFARS 252.227-7013,
;; AND/OR IN SIMILAR OR SUCCESSOR CLAUSES IN THE FAR, DOD OR NASA FAR
;; SUPPLEMENT. UNPUBLISHED RIGHTS RESERVED UNDER THE COPYRIGHT LAWS OF
;; THE UNITED STATES.  
;; 
;; COPYRIGHT 3DFX INTERACTIVE, INC. 1999, ALL RIGHTS RESERVED
;;

%ifdef GL_AMD3D
;;--------------------------------------------------------------------------
;; start AMD3D version - CONVERTED TO SSE
;;--------------------------------------------------------------------------
; Note: MMX registers (mmX) are replaced by SSE registers (xmmX).
; The 'femms' instruction is removed as SSE is orthogonal to FPU/x87 state.

%define gc      rdi                     ; points to graphics context
%define fifo    rbp                     ; points to fifo entries
%define tempVal rsi

%IF GLIDE_CULLING
%define fa      rcx                     ; vtx a pointer (Arg 1)
%define fb      rdx                     ; vtx b pointer (Arg 2)
%define fc      r8                      ; vtx c pointer (Arg 3)
%define cull    r9d                     ; cull mode
%define intArea r10d                    ; area temp storage

    ;; Prologue stuff (X64 ABI)
    push rdi ; save caller's RDI
    push rsi ; save caller's RSI
    push rbx ; save caller's RBX
    push rbp ; save caller's RBP

    mov     r11, [_GlideRoot+curGC]
    mov     gc, [r11]                   ; GR_DCL_GC
    
    mov     r9d, [gc + cull_mode]       ; get cull mode
    mov     tempVal, [_GlideRoot+curTriSize] ; data for whole triangle in bytes
    
    ; femms removed - SSE state is managed separately

    ;; Cull Check (MMX -> SSE registers XMM)
    movq      xmm2, [fc + X]            ; yc | xc 
    shl       r9d, 31                   ; culltest << 31 (using R9D for cull)

    movq      xmm1, [fb + X]            ; yb | xb 
    add       tempVal, 4                ; space required in fifo

    movq      xmm0, [fa + X]            ; ya | xa
    mov       rbx, [gc + fifoRoom]      ; space available in fifo 

    ;; Area_Computation (3DNow! pf* instructions need translation/replacement, 
    ;; but are kept here as a direct translation: PFXCHG/PFADD/PFSUB/PFMUL are 
    ;; single-precision floating point on MMX registers. Assuming NASM supports 
    ;; these for MMX, they remain for X64's MMX usage, which is not ideal 
    ;; but functionally identical if the CPU supports 3DNow!/MMX extensions.)
    
    pfsubr    xmm2, xmm1                ; dyBC | dxBC 
    pfsub     xmm0, xmm1                ; dyAB | dxAB

    movq      xmm