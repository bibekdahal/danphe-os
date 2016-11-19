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


struct pci_device find_pci_device(uint16_t class, uint16_t subclass, uint16_t interface);


void init_pci();