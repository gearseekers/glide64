/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fxpci.h"
#include "fxlinux.h" // <-- FIX: Includes definitions for pci_open_linux, etc.

static int pci_fd = -1; // File descriptor for PCI access

FxBool pciOpen(void) {
    if (pci_fd >= 0) {
        return FXTRUE; // Already open
    }
    pci_fd = pci_open_linux();
    return (pci_fd >= 0);
}

void pciClose(void) {
    if (pci_fd >= 0) {
        pci_close_linux(pci_fd);
        pci_fd = -1;
    }
}

FxU32 pciReadLong(int bus, int dev, int fn, int addr)
{
    FxU32 result = 0;
    if (pci_fd < 0) {
        fprintf(stderr, "pciReadLong: PCI library not open.\n");
        return 0;
    }
    
    if (pci_read_config_long_linux(pci_fd, bus, dev, fn, addr, &result) != 0) {
        fprintf(stderr, "pciReadLong: Failed to read from PCI config space.\n");
        return 0;
    }
    
    return result;
}

void pciWriteLong(int bus, int dev, int fn, int addr, FxU32 data)
{
    if (pci_fd < 0) {
        fprintf(stderr, "pciWriteLong: PCI library not open.\n");
        return;
    }

    if (pci_write_config_long_linux(pci_fd, bus, dev, fn, addr, data) != 0) {
        fprintf(stderr, "pciWriteLong: Failed to write to PCI config space.\n");
    }
}

// Dummy implementation for pciFindCard and other helpers
FxBool pciFindCard(FxU32 vendorId, FxU32 deviceId, int cardnum, pciDevice_t *card_info) {
    return FXFALSE; 
}
FxU32 pciGetBaseAddr(int bus, int dev, int fn, int reg) {
    return 0;
}