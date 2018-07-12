#include <device/pci.h>
#include <device/usb.h>


struct ehci_cap_regs {
    uint8_t cap_length;
    uint8_t reserved;
    uint16_t hci_version;
    uint32_t hcs_params;
    uint32_t hcc_params;
    uint64_t hcsp_port_route;
};


struct ehci_op_regs
{
    uint32_t usb_cmd;
    uint32_t usb_sts;
    uint32_t usb_intr;
    uint32_t frame_index;
    uint32_t ctrl_ds_segment;
    uint32_t periodic_list_base;
    uint32_t async_list_addr;
    uint32_t reserved[9];
    uint32_t config_flag;
    uint32_t* ports;
};


void init_usb() {
    struct pci_device usb = find_pci_device(0x0c, 0x03, 0x20);
    struct pci_bar bar0 = get_pci_bar(usb, 0);
    void* addr = allocate_for_physical_addr(bar0.addr.address);

    struct ehci_cap_regs* cap = addr;
    struct ehci_op_regs* op = addr + cap->cap_length;
}
