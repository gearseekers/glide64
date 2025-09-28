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
##
## [CONVERTED TO X64 / SSE - AT&T SYNTAX]

.file "xdraw2.inc.s"

# X64 ABI Argument Mapping (Linux System V):
# Arg 1: FxU32 *gc => %rdi
# Arg 2: FxU32 *va => %rsi
# Arg 3: FxU32 *vb => %rdx
# Arg 4: FxU32 *vc => %rcx
# Note: Since this is an included file, we assume the calling procedure
# (e.g., _trisetup_cull) handles the prologue and moves arguments here.

# Redefined Pointer/Offset Aliases
X       .EQU 0
Y       .EQU 4
PTR_SIZE .EQU 8  # 64-bit pointer size

# Non-volatile/Caller-Saved Register Mapping
gc      .REG    (%r15)      # Graphics Context (Non-Volatile)
fifo    .REG    (%r14)      # FIFO pointer (Non-Volatile)
dlp     .REG    (%r13)      # Data List Pointer (Non-Volatile)
vertex  .REG    (%r12)      # Current Vertex Pointer (Non-Volatile)

# Argument Register Aliases (Pointers to vertices)
fa      .REG    (%rsi)      # vtx a pointer (Arg 2)
fb      .REG    (%rdx)      # vtx b pointer (Arg 3)
fc      .REG    (%rcx)      # vtx c pointer (Arg 4)

# Prologue assumes standard x64 stack frame setup and register saving
# The caller is responsible for saving RBP, RBX, R12-R15.
# We will explicitly use R12-R15 and ensure the caller saves them.

    ## Prologue setup (Assuming caller pushed RBP, RBX, R12-R15)
    
    # We use R15 for GC and R14 for FIFO (Non-Volatile)
    # RDI holds the incoming GC pointer (Arg 1)
    
    movq    %rdi, gc                    ## GR_DCL_GC: R15 = GC (Arg 1)

.align 16
.AIF \&GLIDE_CULLING EQ 1

# Culling Check Register Aliases (Volatile/Scratch Registers)
cull    .REG    (%r10d)     # Cull mode (32-bit for the flag value)
intArea .REG    (%r9d)      # Area integer storage (32-bit)
tempQ   .REG    (%r8)       # General 64-bit scratch

    # 1. Load vertex pointers and cull mode from the caller's registers
    # (Pointers fa, fb, fc are already in RSI, RDX, RCX from the caller)
    
    movl    cull_mode(gc), %r10d        # Load cull mode (32-bit access)
    shll    $31, cull                   # culltest << 31 (shift in 32-bit register)

    # 2. Area_Computation (SSE Conversion)
    # Area = (xa - xb)*(yb - yc) - (xb - xc)*(ya - yb)
    # We use XMM0-XMM3 for floating point storage (Volatile).

    # Load Vertices X/Y coordinates as single-precision floats (32-bit floats)
    # XMM0: xa | ya | xa | ya
    # XMM1: xb | yb | xb | yb
    # XMM2: xc | yc | xc | yc
    
    movss   X(fa), %xmm0                # xmm0[0] = xa
    movss   Y(fa), %xmm1                # xmm1[0] = ya
    movss   X(fb), %xmm2                # xmm2[0] = xb
    movss   Y(fb), %xmm3                # xmm3[0] = yb
    movss   X(fc), %xmm4                # xmm4[0] = xc
    movss   Y(fc), %xmm5                # xmm5[0] = yc
    
    # Calculate differentials (xb, yb are repeated for calculation convenience)
    
    # XMM0[0] = dxAB = xa - xb
    subss   %xmm2, %xmm0                
    # XMM1[0] = dyAB = ya - yb
    subss   %xmm3, %xmm1                
    
    # XMM2[0] = dxBC = xb - xc
    subss   %xmm4, %xmm2
    # XMM3[0] = dyBC = yb - yc
    subss   %xmm5, %xmm3
    
    # Area = (dxAB * dyBC) - (dxBC * dyAB)

    # XMM4[0] = dxAB * dyBC
    movss   %xmm3, %xmm4                # xmm4[0] = dyBC
    mulss   %xmm0, %xmm4                # xmm4[0] = dxAB * dyBC
    
    # XMM5[0] = dxBC * dyAB
    movss   %xmm1, %xmm5                # xmm5[0] = dyAB
    mulss   %xmm2, %xmm5                # xmm5[0] = dxBC * dyAB
    
    # XMM4[0] = Area = (dxAB * dyBC) - (dxBC * dyAB)
    subss   %xmm5, %xmm4
    
    # Convert result to integer and store in general register
    # We store the result (float) to memory, load it as integer, 
    # then store it back to memory for the next loop part.
    
    movss   %xmm4, zArea                # Store area float to static memory
    movl    zArea, intArea              # Load area float as integer (intArea)
    xorl    %eax, %eax                  # Clear the return value (%eax = 0)

    # 3. Zero Area Triangle Check
    andl    $0x7fffffff, intArea        # if ((j & 0x7FFFFFFF) == 0)
    SYMNAMEOP   jz, __triDone           # area zero, triangle culled

    # 4. Culling Check
    movl    zArea, intArea              # Reload area int value
    xorl    cull, intArea               # if (j ^ (culltest << 31))
    
    SYMNAMEOP   jge, __triDone          # triangle facing away/flat, culled

.AENDI

.align 16
    ## Check to make sure that we have enough room for
    ## the complete triangle packet.
    movl    curTriSize(_GlideRoot), %eax    # %eax = curTriSize
    movq    fifoRoom(gc), %rbx              # %rbx = fifoRoom

    addl    $4, %eax                        # %eax = curTriSize + 4 (header)
    cmpq    %rax, %rbx                      # Check if %rbx >= %rax (fifoRoom >= needed)
   
    SYMNAMEOP jge, __triBegin               # Enough room, proceed.

    # Call _FifoMakeRoom (RCX, RDX, R8 are arguments 1, 2, 3)
    movl    $0, %r8d                        # Arg 3 = __LINE__ (using 0 here as placeholder)
    movl    $0, %edx                        # Arg 2 = NULL function name pointer
    movl    %eax, %ecx                      # Arg 1 = needed size
    
    call    _FifoMakeRoom                   # make fifo room

    # Stack is clean, proceed to triangle assembly
    
    ## Send triangle parameters
    
    movq    %rbx, dlp                   # %rbx (Non-Volatile) is used as dlp (Data List Pointer)
    movq    %rbp, fifo                  # %rbp (Non-Volatile) is used as fifo pointer
    
    # The rest of the logic follows in the included file, now referencing RDX/RBP/RBX, etc.
