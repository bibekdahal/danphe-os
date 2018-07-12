#include <device/pci.h>
#include <device/usb.h>


void init_usb() {
    struct pci_device usb = find_pci_device(0x0c, 0x03, 0x20);
    uint32_t* bar0 = allocate_for_physical_addr((void*)(pci_read_device(usb, 0x010) & ~0xf));
    puthex(*bar0);
}
