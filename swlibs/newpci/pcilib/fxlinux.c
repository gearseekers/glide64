/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
** The H3DK project. All rights reserved.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

#include "fxpci.h"

// ... (other functions) ...

int pci_open_linux(void)
{
    // The /proc/bus/pci interface is generally safe for 64-bit.
    // Ensure file descriptors are handled correctly.
    int fd = open("/proc/bus/pci", O_RDWR);
    if (fd < 0) {
        perror("pci_open_linux");
        return -1;
    }
    return fd;
}

int pci_read_config_long_linux(int fd, int bus, int dev, int fn, int reg, unsigned int *val)
{
    // Use 'long' for the offset to be safe on 64-bit systems.
    long offset = (bus << 16) | (dev << 8) | fn;
    
    if (lseek(fd, offset, SEEK_SET) != offset) {
        perror("lseek");
        return -1;
    }
    
    if (read(fd, val, sizeof(unsigned int)) != sizeof(unsigned int)) {
        perror("read");
        return -1;
    }
    
    return 0;
}