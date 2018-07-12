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

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_write(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data);
uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

static inline uint32_t pci_read_device(struct pci_device device, uint8_t offset) {
    return pci_read(
            device.address.bus, device.address.slot, device.address.function,
            offset);
}

static inline void pci_write_device(struct pci_device device, uint8_t offset, uint32_t data) {
    pci_write(
            device.address.bus, device.address.slot, device.address.function,
            offset, data);
}

struct pci_device find_pci_device(uint16_t class, uint16_t subclass, uint16_t interface);

void init_pci();
