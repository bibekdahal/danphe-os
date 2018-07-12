#include <device/pci.h>


const uint16_t ADDR_PORT = 0xCF8;
const uint16_t DATA_PORT = 0xCFC;

// A device is addressed using bus, slot and func and
// for each device, 256 bytes of configuration space is available.

// The first 6 bits of the offset is used while calculating the address
// to read 4 bytes among this 256 bytes.

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    // Create configuration address
    uint32_t address = (((uint32_t)bus) << 16) | (((uint32_t)slot) << 11) |
              (((uint32_t)func) << 8) | (offset & 0xfc) | ((uint32_t)0x80000000);

    // Write out the address
    outl(ADDR_PORT, address);

    // Read in the data
    return inl(DATA_PORT);
}

void pci_write(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data) {
    // Create configuration address
    uint32_t address = (((uint32_t)bus) << 16) | (((uint32_t)slot) << 11) |
              (((uint32_t)func) << 8) | (offset & 0xfc) | ((uint32_t)0x80000000);

    // Write out the address
    outl(ADDR_PORT, address);

    // Write out the data
    outl(DATA_PORT, data);
}

uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t tmp = pci_read(bus, slot, func, offset);

    // Basically we are only interested in the last two bits of `offset`
    // which determines which two of the four bytes to take.
    // 00 and 01: the last two bytes
    // 10 and 11: the first two bytes
    return (uint16_t)((tmp >> ((offset & 2) * 8)) & 0xffff);
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

struct pci_bar get_pci_bar(struct pci_device device, int index) {
    // Get address and size of BAR
    uint32_t offset = 0x10 + index * 4;
    uint32_t address = read_pci_device(device, offset);

    write_pci_device(device, offset, 0xffffffff);
    uint32_t mask = read_pci_device(device, offset);
    write_pci_device(device, offset, address);

    struct pci_bar bar;

    if (address & 0x01) {
        bar.addr.port = (uint16_t)(address & ~0x3);
        bar.size = (uint16_t)(~(mask & ~0x3) + 1);
    }
    else if (address & 0x04) {
        // Ours is 32 bit system and doesn't support 64bit address space
        // uint32_t offset_high = 0x10 + (index + 1) * 4;
        // uint32_t address_high = read_pci_device(device, offset_high);

        // write_pci_device(device, offset_high, 0xffffffff);
        // uint32_t mask_high = read_pci_device(device, offset_high);
        // write_pci_device(device, offset_high, address_high);

        // bar.addr.address = (void*)(((uint64_t)address_high << 32) | (address & ~0xf));
        // bar.size = ~(((uint64_t)mask_high << 32) | (mask & ~0xf)) + 1;
    }
    else {
        bar.addr.address = (void*)(address & ~0xf);
        bar.size = ~(mask & ~0xf) + 1;
    }

    return bar;
}

// void init_pci() {
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
// }
