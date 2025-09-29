/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include "fxpci.h"
#include <stdint.h> // For uintptr_t

// ... (most of the file remains the same) ...

// Example of a function that needs modification for 64-bit
int pciOpen(void) {
    // ...
    // When mapping memory, ensure we use uintptr_t for base addresses
    // to avoid truncation on a 64-bit system.
    uintptr_t base_addr = pciGetBaseAddr(bus, dev, fn, 0);
    if (base_addr == 0) {
        fprintf(stderr, "Could not get base address\n");
        return 0;
    }

    // Use %p for printing pointer-sized integers like base addresses.
    printf("Base Address: 0x%p\n", (void*)base_addr);

    // ... map the memory using mmap or equivalent ...
    // void *mapped_mem = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, base_addr);
    
    return 1;
}

// Another example: Ensure all configuration reads/writes handle sizes correctly
FxU32 pciReadLong(int bus, int dev, int fn, int addr)
{
    // The underlying Linux/OS calls for reading PCI config space
    // should handle the 32-bit vs 64-bit address space correctly.
    // No changes are needed here unless the library was doing something
    // non-standard like direct port I/O.
    
    // ... (original implementation) ...
    return result;
}