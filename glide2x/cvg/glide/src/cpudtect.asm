;;
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
; 4     5/28/97 8:23a Peter
; Merge w/ original glide source
; 
; 2     3/04/97 9:10p Dow
; Neutered mutiplatform multiheaded monster.
;;
;;

TITLE  cpudtect.asm

; .586P - REPLACED WITH .x64
.x64

; .model FLAT,C - NOT USED IN X64
; Flat memory, mangle publics with leading '_'

;;      Data for data segment goes here
; DATA SEGMENTS ARE USUALLY HANDLED DIFFERENTLY IN X64

;;; Some useful constants
; CPU Type
CPUTypeUnknown  = 0ffffffffh
CPUTypePrePent  = 4h
CPUTypeP5       = 5h    
CPUTypeP6       = 6h    
        
;;; References to external data:
    
.CODE ; X64 uses the .CODE directive for the code segment
;;
;;  _cpu_detect_asm - detect the type of CPU 
;; 
;;  USAGE:
;;
;;  int __cdecl _cpu_detect_asm(void);
;;
;;  returns 4 for non-pen

; X64 FUNCTION DEFINITION
_cpu_detect_asm PROC 
; NOTE: X64 CALLING CONVENTION returns int (32-bit) in EAX, not RAX
; _cpu_detect_asm must be declared as an external function in C/C++ (no C linkage decoration needed for public functions in X64 MASM).

P6Stuff:
    ; .586 - NOT USED IN X64
    push rbp ; Standard prologue, but not strictly required if no stack frame is used
    mov rbp, rsp
    
    ; We must use 64-bit registers (RAX, RCX, etc.) to access the 64-bit stack.
    ; We save non-volatile registers (rbx, rdi, rsi, r12-r15) which are NOT used here,
    ; but we need to preserve EFLAGS which is 64-bit.
    
    push rax ; Use RAX instead of EAX for stack operations
    push rcx ; Use RCX instead of ECX for stack operations
    pushfq ; Push RFLAGS (64-bit equivalent of pushfd)

    ; First, determine whether CPUID instruction is available.
    ; If it's not, then it's a 386 or 486.
    pop rax ; Pop RFLAGS into RAX
    mov rcx, rax ; save original RFLAGS in RCX
    xor rax, 0200000h ; flip ID bit in RFLAGS (bit 21)
    push rax ; put it back on stack
    popfq ; Pop RFLAGS from stack

    pushfq ; get RFLAGS back
    pop rax ; into rax
    xor rax, rcx ; check to see if we could toggle ID
    jnz CheckIntel ; Changed JZ to JNZ for cleaner flow, now jumps to next check

; NotPentium path
NotPentium:
    pop rcx ; Restore RCX
    pop rax ; Restore RAX
    mov rsp, rbp ; Standard epilogue
    pop rbp
    mov eax, 4 ; Return 4
    ret ; Return 

; This label is used if ID bit can be toggled (P5 or P6)
CheckIntel:
    ;
    ; Now determine whether it's an intel P6 CPU.
    ;
    ;;  Is it an Intel CPU?
    xor eax, eax ; eax = 0.
    cpuid ; get cpuid (output is still EAX, EBX, ECX, EDX)
    xor ebx, 0756e6547h ; "Genu" (32-bit operation is fine)
    jnz NotIntel
    xor edx, 049656e69h ; "ineI"
    jnz NotIntel
    xor ecx, 06c65746eh ; "ntel"
    jnz NotIntel ;

    ;;  Verifying architecture family
    mov eax, 1
    cpuid ; get family/model/stepping
    shr eax, 8 ; rid of model & stepping number
    and eax, 0fh ; use only family
    cmp eax, 6
    jl IsP5 ; It's a P5

    ; Else it's a P6
    ;
    ; Intel P6 processor. 
    ; Make sure it supports Memory Type Range Request registers
    ;
IsP6:   
    pop rcx ; Restore RCX
    pop rax ; Restore RAX
    mov rsp, rbp ; Standard epilogue
    pop rbp
    mov eax, 6 ; Return 6
    ret ; return 

IsP5:   
    pop rcx ; Restore RCX
    pop rax ; Restore RAX
    mov rsp, rbp ; Standard epilogue
    pop rbp
    mov eax, 5 ; Return 5
    ret

NotIntel:
    pop rcx ; Restore RCX
    pop rax ; Restore RAX
    mov rsp, rbp ; Standard epilogue
    pop rbp
    mov eax, 0ffffffffh ; Return CPUTypeUnknown
    ret
        
_cpu_detect_asm ENDP

;------------------------------------------------------------------------------   
; this routine sets the precision to single
; which effects all adds, mults, and divs
    align 4                 ; 
    PUBLIC  single_precision_asm
single_precision_asm PROC 
; NOTE: In X64, RET must be 0, as the stack must be balanced on return.
    push rbp
    mov rbp, rsp
    sub rsp, 8 ; Allocate space for FPU word (8-byte alignment required)
    
    fnclex          ; clear pending exceptions    
    fstcw WORD PTR [rsp]
    movsx eax, WORD PTR [rsp] ; Use MOVSX to sign-extend 16-bit word to EAX (32-bit)
    and eax, 0000fcffh  ; clear bits 9:8
    mov WORD PTR [rsp], ax ; Move back as a 16-bit word
    fldcw WORD PTR [rsp]
    
    mov rsp, rbp ; Standard epilogue
    pop rbp
    ret ; Return
single_precision_asm ENDP

;------------------------------------------------------------------------------   
; this routine sets the precision to double
; which effects all adds, mults, and divs
    align 4                 ; 
    PUBLIC  double_precision_asm
double_precision_asm PROC 
; NOTE: In X64, RET must be 0, as the stack must be balanced on return.
    push rbp
    mov rbp, rsp
    sub rsp, 8 ; Allocate space for FPU word (8-byte alignment required)
    
    fnclex          ; clear pending exceptions    
    fstcw WORD PTR [rsp]
    movsx eax, WORD PTR [rsp] ; Use MOVSX to sign-extend 16-bit word to EAX (32-bit)
    and eax, 0000fcffh  ; clear bits 9:8
    or  eax, 000002ffh  ; set 9:8 to 10
    mov WORD PTR [rsp], ax ; Move back as a 16-bit word
    fldcw WORD PTR [rsp]
    
    mov rsp, rbp ; Standard epilogue
    pop rbp
    ret ; Return
double_precision_asm ENDP
    
END