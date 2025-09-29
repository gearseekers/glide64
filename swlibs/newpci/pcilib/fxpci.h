#ifndef _FXPCI_H_
#define _FXPCI_H_

#ifdef __cplusplus
extern "C" {
#endif

// Include the master header to get all the Fx... type definitions.
#include "3dfx.h"

/*
** PCI Device Information Structure
*/
typedef struct {
    int bus;
    int dev;
    int fn;
    FxU32 vendorID;
    FxU32 deviceID;
    FxU32 pciBaseAddr[6];
    FxU32 pciIOMapped[6];
    FxU32 pciBaseSize[6];
    FxU32 pciExpansionRom;
    int irq;
    int class_code;
    int revision;
} pciDevice_t;

/*
** Function Prototypes
*/
FxBool pciOpen(void);
void pciClose(void);
FxBool pciFindCard(FxU32 vendorId, FxU32 deviceId, int cardnum, pciDevice_t *card_info);
FxU32 pciGetBaseAddr(int bus, int dev, int fn, int reg);
FxU32 pciReadLong(int bus, int dev, int fn, int addr);
void pciWriteLong(int bus, int dev, int fn, int addr, FxU32 data);

#ifdef __cplusplus
}
#endif

#endif /* _FXPCI_H_ */