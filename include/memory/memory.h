#pragma once
#include <multiboot.h>
#include <memory/string.h>
#include <memory/alloc.h>


void init_memory(multiboot_info_t* mbt);

// Physical memory management

void pm_init(uint32_t mem_size_in_kb, uint32_t * bitmap_storage_address);

void pm_init_region(uint32_t base_address, uint32_t size);

void pm_deinit_region(uint32_t base_address, uint32_t size);

void* pm_alloc_frame();

void pm_free_frame(void* address);

// Virtual memory management

void setup_kernel_page_dir();
int map_page(void* physical_address, void* virtual_address);
void unmap_page(void* virtual_address);