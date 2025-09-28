## THIS SOFTWARE IS SUBJECT TO COPYRIGHT PROTECTION AND IS OFFERED ONLY
## PURSUANT TO THE 3DFX GLIDE GENERAL PUBLIC LICENSE. THERE IS NO RIGHT
## TO USE THE GLIDE TRADEMARK WITHOUT PRIOR WRITTEN PERMISSION OF 3DFX
## INTERACTIVE, INC. A COPY OF THIS LICENSE MAY BE OBTAINED FROM THE 
## DISTRIBUTOR OR BY CONTACTING 3DFX INTERACTIVE INC(info@3dfx.com). 
## THIS PROGRAM IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER 
## EXPRESSED OR IMPLIED. SEE THE 3DFX GLIDE GENERAL PUBLIC LICENSE FOR A
## FULL TEXT OF THE NON-WARRANTY PROVISIONS.  
## 
## USE, DUPLICATION OR DISCLOSURE BY THE GOVERNMENT IS SUBJECT TO
## RESTRICTIONS AS SET FORTH IN SUBDIVISION (C)(1)(II) OF THE RIGHTS IN
## TECHNICAL DATA AND COMPUTER SOFTWARE CLAUSE AT DFARS 252.227-7013,
## AND/OR IN SIMILAR OR SUCCESSOR CLAUSES IN THE FAR, DOD OR NASA FAR
## SUPPLEMENT. UNPUBLISHED RIGHTS RESERVED UNDER THE COPYRIGHT LAWS OF
## THE UNITED STATES.  
## 
## COPYRIGHT 3DFX INTERACTIVE, INC. 1999, ALL RIGHTS RESERVED
## [CONVERTED TO X64/SSE ASSEMBLY STRUCTURE - AT&T SYNTAX]

.file   "xdraw2.s"
#OPTION OLDSTRUCTS 
# 586P -> Obsolete for x64

.section    .data
.align 16 # Align for XMM data access
One:        .quad 0x3f80000000000000 # 1.0 (as 64-bit float, originally 32-bit: 0x3f800000)
Area:       .quad 0
dxAB:       .quad 0
dxBC:       .quad 0
dyAB:       .quad 0
dyBC:       .quad 0
culltest:   .quad 0

    ## Extra junk to pad out to the next cache line.
bias0:    .quad 0
pad1:    .quad 0
pad2:    .quad 0
pad3:    .quad 0
pad4:    .quad 0
pad5:    .quad 0
pad6:    .quad 0
pad7:    .quad 0
bias1:    .quad 0

# NOTE: Since the data section starts at 'One', subsequent labels are offsets
# We must use 64-bit offsets (8 bytes per original 4-byte DWORD) for data.
# However, since the X/Y fields in the vertex are still FxU32 (4 bytes), 
# those offsets remain 0 and 4.

zArea:      .EQU One + 8     # 8 bytes offset for QWORD
zdxAB:      .EQU One + 16    # 16 bytes offset
zdxBC:      .EQU One + 24
zdyAB:      .EQU One + 32
zdyBC:      .EQU One + 40
zculltest:  .EQU One + 48

### Definitions of cvg regs and glide root structures.
.INCLUDE "fxgasm.h"

## enables/disables trisProcessed and trisDrawn counters
STATS: .equ 1

#--------------------------------------------------------------------------
# Arguments (X64 System V ABI):
# Arg 1 (va): %rdi
# Arg 2 (vb): %rsi
# Arg 3 (vc): %rdx

# STKOFF is obsolete; stack arguments are now registers.
# We will use the original register assignment aliases where possible.

# The registers are re-mapped to 64-bit volatile registers:
# %r12, %r13, %r14, %r15 are non-volatile and MUST be saved.
# %rbx is non-volatile and MUST be saved.
# %rdi, %rsi, %rdx, %rcx are volatile.

X: .equ 0
Y: .equ 4
 
gc: .equ %r13            # Non-volatile register for GC pointer (must be saved)

.text

.MACRO  SAVE_ALL
    pushq   %r15
    pushq   %r14
    pushq   %r13
    pushq   %r12
    pushq   %rbx
    pushq   %rbp
.ENDM

.MACRO  RESTORE_ALL
    popq    %rbp
    popq    %rbx
    popq    %r12
    popq    %r13
    popq    %r14
    popq    %r15
.ENDM

.MACRO  SYMNAME prefix
    .LCULL\prefix:  
.ENDM

.MACRO  SYMNAMEOP op prefix
    \op .LCULL\prefix
.ENDM
                
    .align 4
    .globl  _trisetup_cull
    .type _trisetup_cull, @function
_trisetup_cull: 
    SAVE_ALL
    movq    %rdi, %r14  # Save va in %r14 (fa)
    movq    %rsi, %r15  # Save vb in %r15 (fb)
    movq    %rdx, %r12  # Save vc in %r12 (fc)

    # Now use volatile registers for calculation
    # %rax = temp area, %rdi, %rsi, %rdx, %rcx = temps

GLIDE_CULLING:      .equ 1
GLIDE_PACK_RGB:     .equ 0
GLIDE_PACK_ALPHA:   .equ 0
GLIDE_GENERIC_SETUP: .equ 0

# ----------------------------------------------------------------------
# Logic for Area Calculation and Culling (Equivalent to xdraw2.inc.S content)
# This replaces the FPU stack logic with SSE
# ----------------------------------------------------------------------

    # Set fa=%r14, fb=%r15, fc=%r12 (used in original code block)

    # 1. Load GC pointer (R13) and cull mode (R9D - volatile)
    movq    _GlideRoot+curGC(%rip), %r10 
    movq    %r10, %r13                  # %r13 = gc
    movl    cull_mode(%r13), %r9d       # %r9d = cull_mode

    movl    %r9d, %eax
    shll    $31, %eax                   # %eax = culltest << 31 
    movl    %eax, culltest(%rip)        # Store culltest (32-bit store is fine)

    # Area_Computation using SSE (x87 FPU removed)
    # Target: Area = (xa-xb)*(yc-yb) - (xc-xb)*(ya-yb)

    # Load Vertices (using 32-bit floats, scalar mode)
    # X/Y are 32-bit floats (4 bytes)
    # fa in %r14, fb in %r15, fc in %r12

    # Load xa, ya, xb, yb, xc, yc (as single precision floats)
    movss   X(%r14), %xmm0  # xmm0[0] = xa
    movss   Y(%r14), %xmm1  # xmm1[0] = ya
    movss   X(%r15), %xmm2  # xmm2[0] = xb
    movss   Y(%r15), %xmm3  # xmm3[0] = yb
    movss   X(%r12), %xmm4  # xmm4[0] = xc
    movss   Y(%r12), %xmm5  # xmm5[0] = yc
    
    # Calculate dxAB = xa - xb (xmm6)
    movss   %xmm0, %xmm6
    subss   %xmm2, %xmm6
    movss   %xmm6, dxAB(%rip)   # Store dxAB

    # Calculate dxBC = xb - xc (xmm7)
    movss   %xmm2, %xmm7
    subss   %xmm4, %xmm7
    movss   %xmm7, dxBC(%rip)   # Store dxBC

    # Calculate dyBC = yb - yc (xmm8)
    movss   %xmm3, %xmm8
    subss   %xmm5, %xmm8
    movss   %xmm8, dyBC(%rip)   # Store dyBC

    # Calculate dyAB = ya - yb (xmm9)
    movss   %xmm1, %xmm9
    subss   %xmm3, %xmm9
    movss   %xmm9, dyAB(%rip)   # Store dyAB
    
    # Area = dxAB*dyBC - dxBC*dyAB

    # t0 = dxAB * dyBC
    movss   %xmm6, %xmm10       # xmm10 = dxAB
    mulss   %xmm8, %xmm10       # xmm10 = dxAB * dyBC

    # t1 = dxBC * dyAB
    movss   %xmm7, %xmm11       # xmm11 = dxBC
    mulss   %xmm9, %xmm11       # xmm11 = dxBC * dyAB

    # Area = t0 - t1
    subss   %xmm11, %xmm10      # xmm10 = Area
    movss   %xmm10, Area(%rip)  # Store Area (32-bit float)
    
    # Get Area as 32-bit integer for culling check
    movl    Area(%rip), %r8d            # %r8d = Area (integer representation of float)
    xorl    %eax, %eax                  # Clear the return value (%eax = 0)

    # Zero Area Triangle Check
    movl    %r8d, %ecx                  # Copy Area into %ecx
    andl    $0x7fffffff, %ecx           # if ((j & 0x7FFFFFFF) == 0)
    jz      .LCULL__triDone             # area zero, triangle culled

    # Triangle area check vs culling mode
    movl    culltest(%rip), %r9d        # Reload culltest into %r9d
    xorl    %r9d, %r8d                  # if (j ^ (culltest << 31))
    
    jge     .LCULL__triDone             # triangle facing away from viewer, culled

# ----------------------------------------------------------------------
# Logic for FIFO Room Check (Unchanged logic, registers updated)
# ----------------------------------------------------------------------
    
    .align 32
    # Check to make sure that we have enough room for
    # the complete triangle packet.
    movl    _GlideRoot+curTriSize(%rip), %eax   # %eax = curTriSize
    movq    fifoRoom(%r13), %rbx                # %rbx = fifoRoom (R13 = gc)

    addl    $4, %eax                            # %eax += 4 (header size)
    movq    %rax, %rcx                          # %rcx = space needed (64-bit)
    
    cmpq    %rbx, %rcx                          # Compare fifoRoom to space needed
    jge     .LCULL__triBegin                    # yup, push out triangle data to Voodoo
    
    # Call _FifoMakeRoom (RCX=needed, RDX=0, R8=__LINE__)
    movq    %rcx, %rdi                          # %rdi is volatile, can be used as temp arg holder
    movq    %rdi, %rcx                          # Arg 1: space needed
    xorl    %edx, %edx                          # Arg 2: 0h
    movq    $__LINE__, %r8                      # Arg 3: line number
    call    _FifoMakeRoom

# ----------------------------------------------------------------------
# INCOMPLETE: This section would contain the FIFO writing and parameter loops
#             from the original included file (`xdraw2.inc.S`). 
# ----------------------------------------------------------------------
    
    .align 32
.LCULL__triBegin:
    # ... (rest of the assembly logic, omitted for being part of the included file)
    
.LCULL__triDone:
    # ----------------------------------------------------------------------
    # Epilogue (Updates Stats and Restores Registers)
    # ----------------------------------------------------------------------
    
    # We assume the body of the function finishes setting up the FIFO and returns here.
    # The return value (0 for culled, 1 for drawn) is already in %eax or set here.
    
    # Update trisProcessed
    movl    _GlideRoot+trisProcessed(%rip), %esi # %esi = trisProcessed
    addl    $1, %esi                            # trisProcessed++
    movl    %esi, _GlideRoot+trisProcessed(%rip)
    
    # Return value is already in %eax (0 for culled)
    RESTORE_ALL
    ret

.L_end_trisetup_cull:
    .size _trisetup_cull,.L_end_trisetup_cull-_trisetup_cull

# ----------------------------------------------------------------------
# Subsequent Functions (Stubs for the rest of the file structure)
# ----------------------------------------------------------------------

# The rest of the functions require the same prologue/epilogue and SSE translation
# of the area calculation/packing logic if culling/packing is enabled.
# Since the body of the function is in the missing included file, only stubs are provided.

.MACRO  SYMNAME prefix
    .LNOCULL\prefix:    
.ENDM

.MACRO  SYMNAMEOP op prefix
    \op .LNOCULL\prefix
.ENDM

# ... (rest of the file's structure) ...
