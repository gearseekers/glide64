/*
** Copyright (c) 1999, 2000, 2001, 2002, 2003
** The Glimpse project. All rights reserved.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** The views and conclusions contained in the software and documentation
** are those of the authors and should not be interpreted as representing
** official policies, either expressed or implied, of the Glimpse project.
*/

#include "3dfx.h"
#include "fxpci.h"

/*
** The PciRegister type was not defined.
** Adding the definition here to resolve the compilation errors.
*/
typedef enum {
    READ_ONLY,
    WRITE_ONLY,
    READ_WRITE
} PciRegisterAccess;

typedef struct {
    FxU32 address;
    FxU32 size;
    PciRegisterAccess access;
} PciRegister;

#include "sst1_pci.h"

const PciRegister SST1_PCI_INIT_ENABLE = { 0x40, 4, READ_WRITE };
const PciRegister SST1_PCI_BUS_SNOOP_0 = { 0x44, 4, WRITE_ONLY };
const PciRegister SST1_PCI_BUS_SNOOP_1 = { 0x48, 4, WRITE_ONLY };
const PciRegister SST1_PCI_CFG_STATUS  = { 0x4C, 4, READ_WRITE };

const PciRegister SST1_PCI_VCLK_ENABLE = { 0xc0, 4, WRITE_ONLY };
const PciRegister SST1_PCI_VCLK_DISABLE= { 0xe0, 4, WRITE_ONLY };