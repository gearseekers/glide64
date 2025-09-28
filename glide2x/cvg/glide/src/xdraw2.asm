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
;; [CONVERTED TO X64 ASSEMBLY STRUCTURE]
;;

%include "xos.inc"

;;; Definitions of cvg regs and glide root structures.
%include "fxgasm.h"

extrn   _GlideRoot
extrn   _FifoMakeRoom, 12 ; 12 is usually the number of arguments, which is 
                          ; irrelevant for X64 extern definitions.

%MACRO GR_FIFO_WRITE 3
    ; Use 64-bit registers where necessary, assuming %1 is a 64-bit pointer
    mov     qword [%1 + %2], %3
%ENDMACRO ; GR_FIFO_WRITE

%MACRO WRITE_MM1_FIFO_ALIGNED 0
  %ifdef GL_AMD3D
    ; movq is fine, MM registers are 64-bit
    movq      [fifo], mm1           ; store current param | previous param
  %else
    ;;
  %endif
%ENDMACRO ; WRITE_MM1_FIFO_ALIGNED

%MACRO WRITE_MM1LOW_FIFO 0
  %ifdef GL_AMD3D
    ; movd is fine
    movd      [fifo], mm1           ; store current param | previous param
  %else
    ;;
  %endif
%ENDMACRO ; WRITE_MM1LOW_FIFO

segment   SEG_DATA
    One         DD  1.0
    Area        DD  0
%IF GLIDE_PACKED_RGB
    bias0       DD  0
    bias1       DD  0
%ENDIF

segment   SEG_CONST
$T2003  DD  12288.0
$T2005  DD  1.0
$T2006  DD  256.0

;;; Arguments: X64 parameters are passed in registers: RCX, RDX, R8, R9
; STKOFF  equ 16 ; REMOVED: Stack-based argument passing is not used.
; The three vertex arguments (_va, _vb, _vc) will be in the first three registers.
; Linux ABI: RDI, RSI, RDX (not RCX)
; Windows ABI: RCX, RDX, R8
; Since the code used a C-model, we'll assume a standard ABI and pass in registers.

; The original EQU values for argument offsets are removed.
; The three vertex arguments (_va, _vb, _vc) are assumed to be passed in 
; RCX, RDX, and R8 (Windows ABI) or RDI, RSI, RDX (Linux ABI).

    ;; coordinate offsets into vertex.
    ;; NB:  These are constants and are not
    ;;      user settable like the rest of the
    ;;      parameter offset. Weird.
X       equ 0
Y       equ 4

%MACRO PROC_TYPE 1
  ; All procedures must follow X64 calling conventions (register passing, stack alignment)
  ; Since the procedures take 12 bytes of arguments (3 pointers), 
  ; they take 3 64-bit arguments (RCX, RDX, R8 on Windows, or RDI, RSI, RDX on Linux)

  %ifdef GL_AMD3D
    ; The number '12' previously indicated stack size in bytes. 
    ; It's removed here as parameters are passed in registers.
    proc %1_3DNow
  %else
    proc %1
  %endif
%ENDMACRO ; PROC_TYPE

;; enables/disables trisProcessed and trisDrawn counters
%define STATS 1

segment   SEG_TEXT

            ALIGN 32
PROC_TYPE _trisetup_cull
  ; X64 FUNCTION PROLOGUE
  push rbp
  mov rbp, rsp
  ; The compiler will handle the register arguments (RCX, RDX, R8)

%define GLIDE_CULLING     1
%define GLIDE_PACK_RGB      0
%define GLIDE_PACK_ALPHA    0
%define GLIDE_GENERIC_SETUP 0
%INCLUDE "xdraw2.inc"
%undef GLIDE_GENERIC_SETUP
%undef GLIDE_PACK_ALPHA
%undef GLIDE_PACK_RGB
%undef GLIDE_CULLING

  ; X64 FUNCTION EPILOGUE
  pop rbp
endp

%IF GLIDE_PACKED_RGB

            ALIGN 32
PROC_TYPE _trisetup_cull_rgb
  push rbp
  mov rbp, rsp

%define GLIDE_CULLING     1
%define GLIDE_PACK_RGB      1
%define GLIDE_PACK_ALPHA    0
%define GLIDE_GENERIC_SETUP 0
%INCLUDE "xdraw2.inc"
%undef GLIDE_GENERIC_SETUP
%undef GLIDE_PACK_ALPHA
%undef GLIDE_PACK_RGB
%undef GLIDE_CULLING

  pop rbp
endp

            ALIGN 32
PROC_TYPE _trisetup_cull_argb
  push rbp
  mov rbp, rsp

%define GLIDE_CULLING     1
%define GLIDE_PACK_