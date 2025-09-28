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
;; [CONVERTED TO X64 ASSEMBLY STRUCTURE - NASM/MASM SYNTAX]

%include "xos.inc"

%include "fxgasm.h"

extrn   _GlideRoot

segment SEG_TEXT

; grDrawTriangle (Windows/C++ ABI)
; Arguments: (void *va, void *vb, void *vc) => RCX, RDX, R8
; Since the target function is likely also CDECL, we don't need a custom prologue.

align 16
proc grDrawTriangle 
    ; Load pointer to current GC (curGC) structure from _GlideRoot (64-bit pointer)
    mov rax, qword [_GlideRoot]
    mov rax, qword [rax + curGC]  ; RAX now holds the address of the current GC structure

    ; Load the address of the dispatch routine (kTriProcOffset) from the GC structure.
    ; This is the target address of the triangle setup routine (likely x86 code pointer).
    ; We must ensure the offset kTriProcOffset correctly reflects the 64-bit structure layout.
    mov rax, qword [rax + kTriProcOffset] 
    
    ; Jump to the dispatch routine.
    jmp rax
endp

align 16

%if XOS == XOS_WIN32
%ifdef __MINGW32__
; For X64, the name decoration (@12) is generally removed.
export  grDrawTriangle
%else
; Standard MSVC/MASM X64 function export name is usually undecorated.
export grDrawTriangle
%endif
%endif ; _WIN32