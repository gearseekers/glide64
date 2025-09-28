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
;; [CONVERTED TO X64/SSE ASSEMBLY STRUCTURE - NASM SYNTAX]

%include "xos.inc" ; Use updated x64 configuration

extrn _FifoMakeRoom

;; X64 ABI Argument Mapping (6 parameters, all 32-bit FxU32 or 64-bit pointers):
; Arg 1: FxU32 *gc => RCX (Pointer, becomes GC register)
; Arg 2: FxU32 *baseAddr => RDX (Pointer)
; Arg 3: FxU32 maxS => R8D (32-bit int)
; Arg 4: FxU32 minT => R9D (32-bit int)
; Arg 5: FxU32 maxT => [RSP + 40] (1st stack argument)
; Arg 6: FxU32 *texData => [RSP + 48] (2nd stack argument)

; Define local register usage (Non-Volatile/Caller-Saved must be pushed)
%define fifo        RBP         ; FIFO Pointer (Non-Volatile)
%define gc          RSI         ; Graphics Context Pointer (Non-Volatile)
%define dataPtr     RDI         ; Texture Data Pointer (Non-Volatile)
%define curT        RBX         ; T-Coordinate/Height Counter (Non-Volatile)
%define curS        R12D        ; S-Coordinate/Width Counter (Non-Volatile)
%define fRoom       R13         ; FIFO Room Available (Non-Volatile)
%define maxS_ptr    R14         ; Pointer to store maxS_bytes (Non-Volatile)
%define tempRax     RAX         ; Volatile/Temp

; Stack Offsets for Args 5 & 6 (after 4 register shadows + RBP push)
%define _maxT_STK   40
%define _texData_STK 48

; Replaced MMX macro with standard call
%MACRO _grCommandTransportMakeRoom 3
    ; X64 ABI: RCX, RDX, R8
    mov     rcx, %1 
    mov     rdx, %2
    mov     r8, %3
    call    _FifoMakeRoom
%ENDMACRO ; _grCommandTransportMakeRoom


segment SEG_TEXT

    ALIGN 16

; Combined routine name for X64 standard ABI
proc _grTexDownload_SSE
    ; Prologue: Save non-volatile registers used (RBX, RBP, RDI, RSI, R12, R13, R14)
    push    rbp
    push    rbx
    push    rsi
    push    rdi
    push    r12
    push    r13
    push    r14
    
    ; 1. Load arguments from registers and stack
    mov     curT, qword [RSP + _maxT_STK + 7 * 8] ; curT = maxT (Arg 5)
    mov     tempRax, qword [RSP + _texData_STK + 7 * 8] ; texData (Arg 6)
    mov     dataPtr, tempRax
    
    ; GC pointer (Arg 1: RCX) and minT (Arg 4: R9D)
    mov     gc, rcx
    mov     r14d, r9d ; R14D = minT (using volatile R9D here)
    mov     r15d, r8d ; R15D = maxS (using volatile R8D here)
    
    ; Load maxT and minT from stack into 32-bit registers for calculation
    mov     r9d, dword [RSP + _maxT_STK + 7 * 8] ; maxT
    mov     r8d, dword [RSP + _maxT_STK + 7 * 8 - 4] ; minT (4 bytes below maxT in stack frame)

%IFDEF GLIDE_ALT_TAB
    ; Context check logic - updated registers (removed: mov edx, dword [gc + windowed])
    ; Assuming gc is RSI
    test    rsi, rsi
    je      .dlDone
    mov     r10, qword [rsi+lostContext]
    mov     r11d, dword [r10]
    test    r11d, 1
    jnz     .dlDone
%ENDIF
    
    ; 2. Pre-calculations (using 32-bit parts)
    sub     r9d, r8d                    ; R9D = maxT - minT
    mov     fifo, qword [gc + fifoPtr]  ; fifoPtr (RBP)

    ; maxS is R15D, CurS is R12D
    mov     r12d, r15d                  ; curS = maxS
    add     r9d, 1                      ; R9D = curT = maxT - minT + 1 (using R9D as loop counter)

    ; FEMMS/EMMS replaced by nothing as SSE is used.

    ; Calculate scanline width in bytes (maxS * 4)
    shl     r12d, 2                     ; curS *= 4 (scan line width in bytes)
    mov     r14, qword [RSP + _maxT_STK + 7 * 8 + 4] ; Use stack space for storing scan line width
    mov     dword [r14], r12d           ; Save curS (maxS_bytes)
    mov     r13d, r12d                  ; R13D = curS (width in bytes)

    ; 3. Setup Texture Packet Header (MMX/3DNow! converted to standard logic)
    mov     r10d, r12d                  ; R10D = maxS_bytes
    shr     r10d, 2                     ; R10D = maxS_dword
    
    ; R10D = packetHdr (Texture Port + maxS_dword + packetType 5)
    or      r10d, 0xc0000005 

    ; Calculate Tex Base Address offset (texAddr = baseAddr + minT * 0x200)
    ; baseAddr is RDX, minT is R14D
    mov     r11d, r14d                  ; R11D = minT
    shl     r11d, 9                     ; R11D = minT << 9 (TEX_ROW_ADDR_INCR(minT))

    mov     tempRax, rdx                ; RAX = baseAddr
    add     tempRax, r11                ; RAX = texAddr = baseAddr + minT_incr

    mov     r11, qword [gc + tex_ptr]   ; R11 = gc->tex_ptr
    sub     tempRax, r11                ; RAX = texAddr - gc->tex_ptr (hdr2 value)

    ; RDX/R13D (fRoom) is now used for fifoRoom
    mov     r13, qword [gc + fifoRoom]  ; R13 = fifoRoom (64-bit)

    ; XMM registers for parallel address update (MMX/3DNow! replacement)
    ; XMM0: [TexAddr - gc->tex_ptr] (hdr2) | [packetHdr] (hdr1)
    ; XMM1: [TEX_ROW_ADDR_INCR(1)] | [0] 

    movd    xmm0, r10d                  ; xmm0[0]=packetHdr
    movq    xmm1, tempRax               ; xmm1[0]=hdr2 value (64-bit)
    punpckldq xmm0, xmm1                ; xmm0 = [hdr2 | hdr1]

    ; Calculate TEX_ROW_ADDR_INCR(1) = 0x200
    mov     r11d, 0x200
    movd    xmm1, r11d                  ; xmm1[0]=0x200
    
    ; Final address increment step uses XMM for parallel addition (paddd)
    psllq   xmm1, 32                    ; xmm1 = [0x200 | 0] (as 32-bit ints)

    ; 4. FIFO Alignment Check (RBP=fifo, R13=fRoom)
    test    fifo, 4
    jnz     .handleAlign                ; Not aligned

.startDownload:
    ; Check for enough room before loop (maxS_bytes + 8 header)
    mov     r14d, r12d
    add     r14d, 8
    cmp     r13, r14                    ; fRoom vs needed (32-bit compare is OK)
    jge     .loopT                      ; Enough room, start loop

    ; Need room: Call _FifoMakeRoom
    ; RCX = needed, RDX = 0, R8 = __LINE__
    mov     rcx, r14                    ; space needed
    xor     rdx, rdx
    mov     r8, __LINE__
    _grCommandTransportMakeRoom rcx, rdx, r8

    mov     fifo, qword [gc + fifoPtr]
    mov     r13, qword [gc + fifoRoom]
    mov     r12d, dword [r14]           ; reload curS_bytes
    jmp     .loopT

.handleAlign:
    cmp     r13, 4
    jge     .mmxAlignFifo
    
    _grCommandTransportMakeRoom 4, 0, __LINE__
    
    mov     fifo, qword [gc + fifoPtr]
    mov     r13, qword [gc + fifoRoom]
    mov     r12d, dword [r14] ; reload curS
    
    test    fifo, 4
    jnz     .mmxAlignFifo ; If still unaligned after room call, proceed to write NULL

.mmxAlignFifo:
    mov     dword [fifo], 0
    sub     r13, 4
    mov     qword [gc + fifoRoom], r13
    add     fifo, 4
    
    ; GLIDE_DEBUG checkPtr removed for brevity
    
    mov     qword [gc + fifoPtr], fifo
    jmp     .loopT

    ALIGN 32

.loopT:
    ; 5. Write Header (XMM0 = [hdr2 | hdr1])
    movq    qword [fifo], xmm0          ; store hdr2 | hdr1
    add     fifo, 8                     ; increment fifo ptr (hdr1 + hdr2)

    ; 6. S Coordinate Inner Loop (Unrolled for 8 bytes/2 DWORDS per write)
.loopS:
    ; Load 8 bytes (two 32-bit texels)
    movq    xmm2, qword [dataPtr]       ; load 64 bit data
    add     fifo, 8                     ; pre-increment fifoPtr
    
    add     dataPtr, 8                  ; dataPtr += 8
    sub     r12d, 8                     ; curS -= 8

    movq    qword [fifo - 8], xmm2      ; write texelData
    jnz     .loopS

    ; 7. Update pointers and check T loop
    mov     rcx, qword [gc + fifoPtr]   ; old fifo ptr
    mov     tempRax, fifo               ; new fifo ptr
    mov     qword [gc + fifoPtr], fifo  ; save new fifo ptr
    
    ; GLIDE_DEBUG checkPtr removed for brevity

    sub     tempRax, rcx                ; space used up
    mov     r14d, dword [r14]           ; reload curS_bytes (not needed but follows pattern)
    sub     r13, tempRax                ; update fifoRoom
    sub     r9d, 1                      ; curT--

    mov     qword [gc + fifoRoom], r13  ; save new fifoRoom
    jz      .dlDone

    ; Update address for next line: paddd
    paddd   xmm0, xmm1                  ; texAddr+=TEX_ROW_ADDR_INCR(1) | packetHdr
    
    ; Check for room before looping again
    mov     r14d, r12d
    add     r14d, 8
    cmp     r13, r14
    jge     .loopT
    
    ; Not enough room: Call _FifoMakeRoom (RCX=needed, RDX=0, R8=__LINE__)
    mov     rcx, r14
    xor     rdx, rdx
    mov     r8, __LINE__
    _grCommandTransportMakeRoom rcx, rdx, r8

    mov     fifo, qword [gc + fifoPtr]
    mov     r13, qword [gc + fifoRoom]
    mov     r12d, dword [r14] 
    jmp     .loopT


.dlDone:
    ; Final state cleanup: emms/femms removed (using SSE)
    
    ; Epilogue: Restore non-volatile registers
    pop     r14
    pop     r13
    pop     r12
    pop     rdi
    pop     rsi
    pop     rbx
    pop     rbp
    
    ret
endp
