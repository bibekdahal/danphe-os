#include <system/pci.h>


static uint16_t pci_read_word (uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {

    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
 
    // create configuration address
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    // write out the address
    outl(0xCF8, address);

    // read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32 bits register
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}


static inline uint16_t get_vendor_id(uint16_t bus, uint16_t device, uint16_t function) {
    return pci_read_word(bus, device, function, 0x0);
}


static inline uint16_t get_device_id(uint16_t bus, uint16_t device, uint16_t function) {
    return pci_read_word(bus, device, function, 0x02);
}

static inline uint16_t get_class(uint16_t bus, uint16_t device, uint16_t function) {
    return pci_read_word(bus, device, function, 0x0A) >> 8;
}

static inline uint16_t get_subclass(uint16_t bus, uint16_t device, uint16_t function) {
    return pci_read_word(bus, device, function, 0x0A) & 0xFF;
}

static inline uint16_t get_interface(uint16_t bus, uint16_t device, uint16_t function) {
    return pci_read_word(bus, device, function, 0x08) >> 8;
}


struct pci_device find_pci_device(uint16_t class, uint16_t subclass, uint16_t interface) {
    for(uint16_t bus = 0; bus < 256; bus++) {
        for(uint8_t slot = 0; slot < 32; slot++) {
            for(uint8_t function = 0; function < 8; function++) {

                uint16_t vendor = get_vendor_id(bus, slot, function);
                if(vendor == 0xffff)
                    continue;

                uint16_t device = get_device_id(bus, slot, function);
                uint16_t _class = get_class(bus, slot, function);
                uint16_t _subclass = get_subclass(bus, slot, function);
                uint16_t _interface = get_interface(bus, slot, function);
                
                if (class == _class && subclass == _subclass && interface == _interface) {
                    struct pci_device pdevice;
                    pdevice.valid = 1;
                    pdevice.address.bus = bus;
                    pdevice.address.slot = slot;
                    pdevice.address.function = function;
                    pdevice.vendor = vendor;
                    pdevice.device = device;
                    return pdevice;
                }
            }
        }
    }
    struct pci_device pdevice;
    pdevice.valid = 0;
    return pdevice;
}

 void init_pci() {
    // for(uint32_t bus = 0; bus < 256; bus++) {
    //     for(uint32_t slot = 0; slot < 32; slot++) {
    //         for(uint32_t function = 0; function < 8; function++) {

    //                 uint16_t vendor = get_vendor_id(bus, slot, function);
    //                 if(vendor == 0xffff)
    //                     continue;

    //                 uint16_t device = get_device_id(bus, slot, function);
    //                 puts("\nVendor: 0x"); puthex(vendor);
    //                 puts("\tDevice: 0x"); puthex(device);

    //                 puts("\nClass: 0x"); puthex(get_class(bus, slot, function));
    //                 puts("\tSubclass: 0x"); puthex(get_subclass(bus, slot, function));
    //                 puts("\tInterface: 0x"); puthex(get_interface(bus, slot, function));
                    
    //         }
    //     }
    // }
}
