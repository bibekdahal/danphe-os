#include <memory/memory.h>


void kernel_phys_end_var();
void kernel_virt_end_var();


void init_memory(multiboot_info_t* mbt) {
    // First init physical memory manager

    // Get kernel end address
    uint32_t kernel_phys_end = (uint32_t) &kernel_phys_end_var;
    uint32_t kernel_virt_end = (uint32_t) &kernel_virt_end_var;

    // Put the physical memory manager at just after the kernel
    uint32_t mem_size = 0x400 + mbt->mem_upper;     // = 1MB + Upper memory size
	pm_init(mem_size, (uint32_t*)kernel_virt_end);

    // Get available physical memory space and mark them all free
    memory_map_t* mmap = (memory_map_t*) (mbt->mmap_addr + VIRTUAL_KERNEL_BASE);
	while((uint32_t)mmap < mbt->mmap_addr + mbt->mmap_length + VIRTUAL_KERNEL_BASE) {
		if (mmap->type == 1) {
            pm_init_region(mmap->base_addr_low, mmap->length_low);
        }

		mmap = (memory_map_t*) ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
	}

    // Also mark from start of memory to end of kernel as taken
	pm_deinit_region(0, kernel_phys_end+4096);  // Size = END_ADDRESS + 1 frame


    // Next (re)setup kernel page directory

    setup_kernel_page_dir();

    // Initialize allocation system, including marking free and available pages

    // First free all pages
    memset(&active_memory.pages[0], 0, 1024*1024/8);
    // For kernel we will only be using higher half memory
    // So reserve all pages up to the one where kernel is currently at
    memset(&active_memory.pages[0], 0xFF, (VIRTUAL_KERNEL_BASE/4096+1024)/8);
    // Last page table, used for recursive page directory
    memset(&active_memory.pages[(1024*1024-1024)/32], 0xFF, 1024/8);

    // Now initialize the block list in the memory
    active_memory.first_block = 0;
    malloc(0);          // To create the first free block
}