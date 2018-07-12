#pragma once
#include <system/system.h>


struct pci_device_address {
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
};

struct pci_device {
    uint8_t valid;
    struct pci_device_address address;
    uint16_t vendor;
    uint16_t device;
};

struct pci_bar {
    union {
        void* address;
        uint16_t port;
    } addr;
    uint64_t size;
};

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_write(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data);
uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

struct pci_device find_pci_device(uint16_t class_, uint16_t subclass, uint16_t interface);

static inline uint32_t read_pci_device(struct pci_device device, uint8_t offset) {
    return pci_read(
            device.address.bus, device.address.slot, device.address.function,
            offset);
}

static inline void write_pci_device(struct pci_device device, uint8_t offset, uint32_t data) {
    pci_write(
            device.address.bus, device.address.slot, device.address.function,
            offset, data);
}

struct pci_bar get_pci_bar(struct pci_device device, int index);
