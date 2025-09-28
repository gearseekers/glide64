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

;
; compulsory header for glide3/xdraw* assembly specializations (NASM)
; [CONVERTED TO X64/AMD64 ARCHITECTURE]
;

;---------------------------------------
; platform defines (32-bit values used for identification)
;---------------------------------------
%define XOS_DJGPP 1
%define XOS_LINUX 2
%define XOS_WIN32 4
%define XOS_WATCD 8

; Define X64-specific flags
%define XOS_LINUX64 16
%define XOS_WIN64 32

%define CDECL_ABI 0   ; Linux/Unix style register passing
%define MS_ABI 1      ; Microsoft style register passing

;---------------------------------------
; pick up the right OS and ABI (using 64-bit detection)
;---------------------------------------
%ifdef __x86_64__
  ; We are compiling for 64-bit architecture
  %ifdef __linux__
    %define XOS XOS_LINUX64
    %define ABI CDECL_ABI
    %define ELFTYPE 1
  %elifdef __WIN64__
    %define XOS XOS_WIN64
    %define ABI MS_ABI
  %else
    %error Unknown 64-bit OS
  %endif
%else
  ; We are compiling for 32-bit architecture (original targets)
  %ifdef __DJGPP__
    %define XOS XOS_DJGPP
    %define ABI CDECL_ABI
  %elifdef __linux__
    %define XOS XOS_LINUX
    %define ABI CDECL_ABI
    %define ELFTYPE 1
  %elifdef __WIN32__
    %define XOS XOS_WIN32
    %define ABI MS_ABI
  %elifdef __WATCOMD__
    %define XOS XOS_WATCD
    %define ABI MS_ABI
  %else
    %error Unknown 32-bit OS
  %endif
%endif

;---------------------------------------
; general purpose macros for X64 (ABI dependent)
; NOTE: Stack counting (@N) is removed as arguments pass in registers.
;---------------------------------------

; EXTRN: Declares external symbol. Stack count removed.
%macro	extrn	1
    extern	%1
%endmacro

; GLOBL: Declares global symbol. Stack count removed.
%macro	globl	1
    global	%1
%endmacro

; PROC/ENDP: Defines a procedure. In X64, arguments are registers.
%macro		proc	1
	%push	proc
    ; In X64, stack return cleanup is not needed (RET), 
    ; or is handled by RET N in some specific cases, but generally RET.
	%if ELFTYPE
		globl	%1:function
	%else
		globl	%1
	%endif
%1:
%endmacro

%macro		endp	0
	%ifnctx	proc
		%error	Mismatched ENDP/PROC
	%else
		%pop
	%endif
%endmacro

; RET: In X64, RET always returns without stack cleanup by the callee.
%macro		ret	0
	RET
%endmacro

; INVOKE: Simplified for X64. Arguments must be pushed onto the stack 
; or moved into RCX, RDX, R8, R9 before the call.
; This complex 32-bit stack cleanup macro is replaced by simple argument moves 
; if the function parameters are known, but here we just convert the structure.
; In pure x64 CDECL, the caller is responsible for args RCX/RDX/R8/R9/stack.
%macro invoke 1-*
	; This macro should be replaced with explicit MOV REX, argument
	; followed by a CALL. Keeping the structure but removing 32-bit cleanup.
	%rep	%0 - 1
	%rotate -1
	; Argument 1 is %1, arguments 2..N are rotated into %1 position
	; Note: This macro is highly complex to convert dynamically. 
	; For X64, the assembly source must manually load RCX/RDX/R8/R9.
	; We retain the basic structure but assume manual register setup.
	; If the original code implicitly used PUSH for args, that must change.
	; PUSH %1 
	%endrep
	%rotate -1
	call	%1
    ; No stack cleanup needed by the caller in X64 ABI (caller cleans up stack for CDECL, 
    ; but registers are used first in x64, which is easier).
%endmacro


;---------------------------------------
; Windows X64
;---------------------------------------
%if XOS == XOS_WIN64

; X64 segments and alignment
%define SEG_TEXT  .text   align=16
%define SEG_DATA  .data   align=16
%define SEG_CONST .rdata  align=16

%macro GET_GC 0
	; GC is typically a global variable or structure member, 
    ; accessed via 64-bit addressing (RIP-relative or absolute).
	mov	rax, qword [_GlideRoot + curGC] ; Load 64-bit GC pointer
	mov	gc, rax ; Assuming 'gc' is defined as a 64-bit register (e.g., RDI)
%endmacro

%endif

;---------------------------------------
; Linux X64 (ELF)
;---------------------------------------
%if XOS == XOS_LINUX64

%define SEG_TEXT  .text   align=16
%define SEG_DATA  .data   align=16
%define SEG_CONST .rodata align=16

%macro GET_GC 0
	; GC is typically a global variable or structure member.
	mov	rax, qword [_GlideRoot + curGC]
	mov	gc, rax ; Assuming 'gc' is defined as a 64-bit register
%endmacro

%endif

;---------------------------------------
; Remaining 32-bit definitions (kept for completeness, assuming x64 is primary target)
;---------------------------------------
; Note: These branches should ideally not be hit if targeting x64.

%if XOS == XOS_DJGPP
%define SEG_TEXT  .text
%define SEG_DATA  .data
%define SEG_CONST .rodata
%macro GET_GC 0
	mov	gc, [_GlideRoot + curGC]
%endmacro
%endif

%if XOS == XOS_LINUX
%define SEG_TEXT  .text   align=32
%define SEG_DATA  .data   align=32
%define SEG_CONST .rodata align=32
%macro GET_GC 0
	mov	gc, [_GlideRoot + curGC]
%endmacro
%endif

%if XOS == XOS_WIN32
%define SEG_TEXT  .text   align=32
%define SEG_DATA  .data   align=32
%define SEG_CONST .rdata  align=32
%macro GET_GC 0
	mov	gc, [_GlideRoot + curGC]
%endmacro
%endif

%if XOS == XOS_WATCD
%define SEG_TEXT  _TEXT	align=1 public use32 class=CODE FLAT
%define SEG_DATA  _DATA	align=4 public use32 class=DATA FLAT
%define SEG_CONST CONST2	align=4 public use32 class=DATA FLAT
%macro GET_GC 0
	mov	gc, [_GlideRoot + curGC]
%endmacro
%endif
