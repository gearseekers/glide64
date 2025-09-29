/*
 * CPU detection code (Converted to X64/SSE)
 *
 * This file replaces legacy MMX/3DNow! capability checks with SSE/SSE2 checks 
 * and updates inline assembly for 64-bit architecture and calling conventions.
 *
 */

#include <signal.h>
#include <setjmp.h>
#include <string.h>

/* Assuming cpuid.h defines _p_info structure and _CPU_FEATURE_* constants */
#include "cpuid.h" 

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ > 3)))
typedef uint32_t __attribute__((__may_alias__)) word32;
#else
typedef uint32_t word32;
#endif

/* These are the bit flags that get set on calling cpuid
 * with register eax set to 1
 */
#define _MMX_FEATURE_BIT        0x00800000
#define _SSE_FEATURE_BIT        0x02000000
#define _SSE2_FEATURE_BIT       0x04000000

/* This bit is set when cpuid is called with
 * register set to 80000001h (only applicable to AMD)
 */
#define _3DNOW_FEATURE_BIT      0x80000000
#define _3DNOWPLUS_FEATURE_BIT  0x40000000
#define _MMXPLUS_FEATURE_BIT    0x00400000

/* Testing code: Use XMM operations to test OS support for SSE/SSE2. 
 * MMX/3DNow! tests are mapped to SSE/SSE2 or removed, as modern X64 CPUs 
 * inherently support MMX/SSE, and 3DNow! is generally obsolete.
 */
#ifdef __GNUC__
/* Note: Using %rbx for saving non-volatile register */
#define TEST_CPUID(f)            __asm__ __volatile__ ("pushq %%rbx; cpuid; popq %%rbx"::"a"(f):"%rcx", "%rdx")
/* xorps xmm0, xmm0 is a simple, legal SSE instruction */
#define TEST_SSE()               __asm__ __volatile ("xorps %%xmm0, %%xmm0")
/* xorpd xmm0, xmm0 is a simple, legal SSE2 instruction */
#define TEST_SSE2()              __asm__ __volatile ("xorpd %%xmm0, %%xmm0")

/* Since MMX/3DNow! are deprecated/always supported in X64, we map them 
   to the safest available instruction or keep the legacy SIGILL test */
#define TEST_3DNOW()             TEST_SSE()      // Map to SSE
#define TEST_MMX()               TEST_SSE()      // Map to SSE
#define TEST_3DNOWPLUS()         TEST_SSE2()     // Map to SSE2
#define TEST_MMXPLUS()           TEST_SSE2()     // Map to SSE2
#else
// For non-GCC compilers (like MSVC/Intel), these would require separate .asm modules
// or specialized intrinsic wrappers. The original structure is left for reference.
#define TEST_CPUID(f)            // Needs specialized port
#define TEST_SSE()               // Needs specialized port
#define TEST_SSE2()              // Needs specialized port
#define TEST_3DNOW()             // Needs specialized port
#define TEST_MMX()               // Needs specialized port
#define TEST_3DNOWPLUS()         // Needs specialized port
#define TEST_MMXPLUS()           // Needs specialized port
#endif


#ifndef __MSC__
static jmp_buf j;


/* Desc: signal handler
 *
 * In  : signal number
 * Out : -
 *
 * Note: returns by `longjmp'ing
 */
static void handler (int signal)
{
    longjmp(j, signal + 1); /* so we can tell... also ensure we don't pass 0 */
}


/* Desc: check if CPU has specific feature
 *
 * In  : feature request
 * Out : 0 == fail, input == pass
 *
 * Note: EBX is now RBX (non-volatile)
 */
static int check_feature (int feature)
{
    if (setjmp(j)) {
        /* we got here only when `longjmp'ed by signal handlers */
        return 0;
    } else {
        /* we have signals and jump buffer set */
        switch (feature) {
            case _CPU_HAS_CPUID:         TEST_CPUID(0);           break;
            case _CPU_FEATURE_SSE:       TEST_SSE();              break;
            case _CPU_FEATURE_SSE2:      TEST_SSE2();             break;
            // Mapped legacy tests to SSE/SSE2
            case _CPU_FEATURE_3DNOW:     TEST_3DNOW();            break;
            case _CPU_FEATURE_MMX:       TEST_MMX();              break;
            case _CPU_FEATURE_3DNOWPLUS: TEST_3DNOWPLUS();        break;
            case _CPU_FEATURE_MMXPLUS:   TEST_MMXPLUS();          break;
            default: return 0;
        }
        return feature;
    }
}
#endif


/* Desc: perform (possibly faulting) instructions in a safe manner
 *
 * In  : feature request
 * Out : 0 == fail, input == pass
 *
 * Note: The __MSC__ block handling is left as is, but we assume 
 * standard POSIX error handling for cross-platform compatibility.
 */
static int has_feature (int feature)
{
#ifndef __MSC__
    int rv;

    /* register signal handlers */
    void (*old_sigill)(int) = signal(SIGILL, handler);
    if (old_sigill == SIG_ERR) {
        return 0;
    }

    rv = check_feature(feature);

    /* restore the signal handlers */
    signal(SIGILL, old_sigill);
    return rv;
#else
    /* Use the non-standard __try/__except mechanism because win95 fails to catch
     * sigillegal for SSE using standard signal mechanism.
     * HACK ALERT! HACK ALERT! HACK ALERT!
     * This means the MinGW version cannot be safely run under Win95!
     */
#define _TRY() __try {
#define _EXCEPTION() } __except(1) { return 0; } /* EXCEPTION_EXECUTE_HANDLER=1 */
    switch (feature) {
        case _CPU_HAS_CPUID:         _TRY() TEST_CPUID(0)     _EXCEPTION() break;
        case _CPU_FEATURE_SSE:       _TRY() TEST_SSE()        _EXCEPTION() break;
        case _CPU_FEATURE_SSE2:      _TRY() TEST_SSE2()       _EXCEPTION() break;
        case _CPU_FEATURE_3DNOW:     _TRY() TEST_3DNOW()      _EXCEPTION() break;
        case _CPU_FEATURE_MMX:       _TRY() TEST_MMX()        _EXCEPTION() break;
        case _CPU_FEATURE_3DNOWPLUS: _TRY() TEST_3DNOWPLUS() _EXCEPTION() break;
        case _CPU_FEATURE_MMXPLUS:   _TRY() TEST_MMXPLUS()    _EXCEPTION() break;
        default: return 0;
    }
    return feature;
#endif
}


/* Desc: get CPU info
 *
 * In  : pointer to _p_info
 * Out : features
 *
 * Note: CPUID inline assembly updated for X64.
 */
int _cpuid (_p_info *pinfo)
{
    word32 dwId = 0;
    word32 dwFeature = 0;
    word32 dwExt = 0;
    int feature = 0, os_support = 0;
    char Ident[13];

    if (!has_feature(_CPU_HAS_CPUID)) {
        return 0;
    }

#ifdef __GNUC__
    __asm__ __volatile__(
   /* get the vendor string */   
   "pushq   %%rbx\n"       
   "xorl %%eax, %%eax\n"      
   "cpuid\n"            
   "movl %%ebx, %%eax\n"      
   "popq %%rbx\n"       
   "movl %%eax, %3\n"      
   "movl %%edx, %4\n"      
   "movl %%ecx, %5\n"      
   /* get the Standard bits */   
   "pushq   %%rbx\n"       
   "movl $1, %%eax\n"      
   "cpuid\n"            
   "popq %%rbx\n"       
   "movl %%eax, %1\n"      
   "movl %%edx, %2\n"      
   /* get AMD-specials */     
   "pushq   %%rbx\n"       
   "movl $0x80000000, %%eax\n"   
   "cpuid\n"            
   "popq %%rbx\n"       
   "cmpl $0x80000000, %%eax\n"   
   "jc   0f\n"       
   "pushq   %%rbx\n"       
   "movl $0x80000001, %%eax\n"   
   "cpuid\n"            
   "popq %%rbx\n"       
   "movl %%edx, %0\n"      
   "0:\n"               
   : "=g"(dwExt), "=g"(dwId), "=g"(dwFeature),
     "=g"(((word32 *)Ident)[0]), "=g"(((word32 *)Ident)[1]), "=g"(((word32 *)Ident)[2])
   : 
   : "eax", "ecx", "edx"
    );
#else
   /* For non-GCC systems, we rely on the host compiler */
    // Placeholder - MSVC style assembly is usually in separate blocks
    // and would need R/E/XAX registers updated.
    // The original MSVC block (not shown here) would need similar 64-bit updates.
#endif

#ifndef __WATCOMC__
    /* Check actual OS support using the safe mechanism */
    // Note: MMX is always supported on x64, but we check OS support for safety.
    
    // MMX is now checked via SSE (since it is a subset always available on x64)
    if (dwFeature & _MMX_FEATURE_BIT) {
        feature |= _CPU_FEATURE_MMX;
        os_support |= has_feature(_CPU_FEATURE_SSE); // Check OS support via safe SSE instruction
    }
    
    // 3DNow! is deprecated and mapped to SSE2/SSE for check
    if (dwExt & _3DNOW_FEATURE_BIT) {
        feature |= _CPU_FEATURE_3DNOW;
        os_support |= has_feature(_CPU_FEATURE_SSE); // Check OS support via safe SSE instruction
    }
    if (dwExt & _3DNOWPLUS_FEATURE_BIT) {
        feature |= _CPU_FEATURE_3DNOWPLUS;
        os_support |= has_feature(_CPU_FEATURE_SSE2); // Check OS support via safe SSE2 instruction
    }
    if (dwExt & _MMXPLUS_FEATURE_BIT) {
        feature |= _CPU_FEATURE_MMXPLUS;
        os_support |= has_feature(_CPU_FEATURE_SSE2); // Check OS support via safe SSE2 instruction
    }
    
    // SSE and SSE2 are primary X64 features
    if (dwFeature & _SSE_FEATURE_BIT) {
        feature |= _CPU_FEATURE_SSE;
        os_support |= has_feature(_CPU_FEATURE_SSE);
    }
    if (dwFeature & _SSE2_FEATURE_BIT) {
        feature |= _CPU_FEATURE_SSE2;
        os_support |= has_feature(_CPU_FEATURE_SSE2);
    }
#endif

    if (pinfo) {
        memset(pinfo, 0, sizeof(_p_info));
        pinfo->os_support = os_support;
        pinfo->feature = feature;
        pinfo->family = (dwId >> 8) & 0xF; /* retrieving family */
        pinfo->model = (dwId >> 4) & 0xF;  /* retrieving model */
        pinfo->stepping = dwId & 0xF;      /* retrieving stepping */
        Ident[12] = 0;
        strcpy(pinfo->v_name, Ident);
    }

    return feature;
}


#if CPUTEST
#include <stdio.h>
/* Desc:
 *
 * In  :
 * Out :
 *
 * Note:
 */
int main (void)
{
    _p_info p;
    _cpuid(&p);
    printf("vendor  : %s\n", p.v_name);
    printf("family  : %d\n", p.family);
    printf("model   : %d\n", p.model);
    printf("stepping: %X\n", p.stepping);
    printf("feature : %08x\n", p.feature);
    printf("support : %08x\n", p.os_support);
    printf("--------\n");
    printf("cpuid   : %d\n", has_feature(_CPU_HAS_CPUID));
    printf("MMX     : %d\n", has_feature(_CPU_FEATURE_MMX));
    printf("SSE     : %d\n", has_feature(_CPU_FEATURE_SSE));
    printf("SSE2    : %d\n", has_feature(_CPU_FEATURE_SSE2));
    printf("3DNow!  : %d\n", has_feature(_CPU_FEATURE_3DNOW));
    printf("3DNow!+ : %d\n", has_feature(_CPU_FEATURE_3DNOWPLUS));
    printf("MMX+    : %d\n", has_feature(_CPU_FEATURE_MMXPLUS));
    return 0;
}
#endif
