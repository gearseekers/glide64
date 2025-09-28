#
# THIS SOFTWARE IS SUBJECT TO COPYRIGHT PROTECTION AND IS OFFERED ONLY
# PURSUANT TO THE 3DFX GLIDE GENERAL PUBLIC LICENSE. THERE IS NO RIGHT
# TO USE THE GLIDE TRADEMARK WITHOUT PRIOR WRITTEN PERMISSION OF 3DFX
# INTERACTIVE, INC. A COPY OF THIS LICENSE MAY BE OBTAINED FROM THE 
# DISTRIBUTOR OR BY CONTACTING 3DFX INTERACTIVE INC(info@3dfx.com). 
# THIS PROGRAM IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER 
# EXPRESSED OR IMPLIED. SEE THE 3DFX GLIDE GENERAL PUBLIC LICENSE FOR A
# FULL TEXT OF THE NON-WARRANTY PROVISIONS.  
# 
# USE, DUPLICATION OR DISCLOSURE BY THE GOVERNMENT IS SUBJECT TO
# RESTRICTIONS AS SET FORTH IN SUBDIVISION (C)(1)(II) OF THE RIGHTS IN
# TECHNICAL DATA AND COMPUTER SOFTWARE CLAUSE AT DFARS 252.227-7013,
# AND/OR IN SIMILAR OR SUCCESSOR CLAUSES IN THE FAR, DOD OR NASA FAR
# SUPPLEMENT. UNPUBLISHED RIGHTS RESERVED UNDER THE COPYRIGHT LAWS OF
# THE UNITED STATES.  
# 
# COPYRIGHT 3DFX INTERACTIVE, INC. 1999, ALL RIGHTS RESERVED
#
# 2     3/04/97 9:10p Dow
# Neutered mutiplatform multiheaded monster.
#
#

.file "cpudtect.s"

# .586P / .model FLAT,C - Removed for x64
.text
.align 4

# CPU Type Constants - Use 32-bit (CPUTypeUnknown) or 64-bit (other) constants.
CPUTypeUnknown  .equ 0xffffffff
CPUTypePrePent  .equ 4
CPUTypeP5       .equ 5
CPUTypeP6       .equ 6

# .globl is the GNU standard for making a function public/external
.globl _cpu_detect_asm
_cpu_detect_asm:        
P6Stuff:
    # Save the *non-volatile* caller-saved registers that we will modify:
    # Only %rbx is used below and is non-volatile in System V ABI.
    pushq   %rbx
    # The return value is 32-bit, placed in %eax.

    # ----------------------------------------------------------------------
    # 1. Check for CPUID Support (Toggle ID bit in RFLAGS)
    # ----------------------------------------------------------------------
    # pusha/popa and pushf/popf become pushq/popq/pushfq/popfq with 64-bit regs.
    
    pushfq                                 # push original RFLAGS (64-bit)
    popq    %rax                           # pop into %rax
    movq    %rax, %rcx                     # save original RFLAGS in %rcx
    xorl    $0x200000, %eax                # flip ID bit (bit 21) in %eax (32-bit operation is fine)
    pushq   %rax                           # put it back on stack
    popfq                                  # pop into RFLAGS
    pushfq                                 # get RFLAGS back
    popq    %rax                           # into %rax
    xorq    %rcx, %rax                     # check if toggle was successful
    jz      NotPentium                     # Sorry, not P5 or P6.
    
    # ----------------------------------------------------------------------
    # 2. Check for "GenuineIntel" signature (always a 32-bit check)
    # ----------------------------------------------------------------------
    xorl    %eax, %eax                     # %eax = 0.
    cpuid                                  # get cpuid (output in %eax, %ebx, %ecx, %edx)
    xorl    $0x756e6547, %ebx              # "Genu"
    jnz     NotIntel
    xorl    $0x49656e69, %edx              # "ineI"
    jnz     NotIntel
    xorl    $0x6c65746e, %ecx              # "ntel"
    jnz     NotIntel                       
    
    # ----------------------------------------------------------------------
    # 3. Check for Family (5 or 6)
    # ----------------------------------------------------------------------
    movl    $1, %eax                       # set %eax=1 for family check
    cpuid                                  # get family/model/stepping
    shrl    $8, %eax                       # rid of model & stepping number
    andl    $0xf, %eax                     # use only family
    cmpl    $6, %eax
    jl      IsP5                           # It's a P5
    
    # Else it's a P6
IsP6:   
    movl    $CPUTypeP6, %eax               # Return 6
    jmp     EndDetect

IsP5:   
    movl    $CPUTypeP5, %eax               # Return 5
    jmp     EndDetect

NotPentium:
    movl    $CPUTypePrePent, %eax          # Return 4
    jmp     EndDetect

NotIntel:
    movl    $CPUTypeUnknown, %eax          # Return 0xffffffff

EndDetect:
    popq    %rbx                           # Restore non-volatile %rbx
    ret                                    # Return. The result is in %eax.

.L_end__cpu_detect_asm:
.size _cpu_detect_asm,.L_end__cpu_detect_asm-_cpu_detect_asm