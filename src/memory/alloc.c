#include <memory/memory.h>

// First a simple page bitmap handler
static inline void set_page(uint32_t page_index) {
    active_memory.pages[page_index/32] |= (1 << (page_index % 32));
}

static inline void unset_page(uint32_t page_index) {
    active_memory.pages[page_index/32] &= ~(1 << (page_index % 32));
}

static inline int test_page(uint32_t page_index) {
    return active_memory.pages[page_index/32] & (1 << (page_index % 32));
}

static int get_first_free_page() {
    uint32_t i, j;
    uint32_t num_pages = 1024*1024;
    for (i=0; i<num_pages/32; ++i) {
        if (active_memory.pages[i] != 0xFFFFFFFF) {
            for (j=0; j<32; ++j) {
                if (!(active_memory.pages[i] & (1 << j))) {
                    return i*32 + j;
                }
            }
        }
    }
    return -1;
}

// Finally a page allocation function
void* allocate_new_page() {
    int i = get_first_free_page();
    if (i < 0)
        return 0;

    void* phys = pm_alloc_frame();
    void* virt = (void*)(i * 4096);

    if (map_page(phys, virt)) {
        set_page(i);
        return virt;
    }

    return 0;
}
 

// Now the actual allocation, deallocation system.

struct Memory active_memory;

static const uint32_t FREE_BLOCK = 0xc001d00d;
static const uint32_t ALLOC_BLOCK = 0xdeadd00d; 


// Check if two block headers represent continuous memory, one after another
static int is_continuous(struct MemoryBlockHeader* one, struct MemoryBlockHeader* two) {
    return ((uint32_t)one + one->size + sizeof(struct MemoryBlockHeader)) == (uint32_t)two;
}


// Merge the prev and next free blocks for a given free block if they are continuous
static struct MemoryBlockHeader* merge_blocks(struct MemoryBlockHeader* block) {

    if (block->magic == FREE_BLOCK) {
        // First merge with the next block
        if (block->next && block->next->magic == FREE_BLOCK) {
            if (is_continuous(block, block->next)) {
                // Merge
                block->size += block->next->size + sizeof(struct MemoryBlockHeader);
                block->next = block->next->next;

                if (block->next)
                    block->next->prev = block;
            }
        }

        // Then merge with the previous block
        if (block->prev && block->prev->magic == FREE_BLOCK) {
            if (is_continuous(block->prev, block)) {
                block = block->prev;
                // Merge
                block->size += block->next->size + sizeof(struct MemoryBlockHeader);
                block->next = block->next->next;

                if (block->next)
                    block->next->prev = block;
            }
        }
    }

    return block;
}


// Create a new block at given address
static struct MemoryBlockHeader* new_free_block(void* address, uint32_t block_size,
                                                struct MemoryBlockHeader* prev,
                                                struct MemoryBlockHeader* next)
{
    struct MemoryBlockHeader* block = (struct MemoryBlockHeader*) address;
    block->magic = FREE_BLOCK;
    block->size = block_size - sizeof(struct MemoryBlockHeader);
    block->next = next;
    block->prev = prev;

    if (next)
        next->prev = block;
    if (prev)
        prev->next = block;

    return merge_blocks(block);
}


// Get next block fater given block, creating new one, if it doesn't exist
static struct MemoryBlockHeader* next_block(struct MemoryBlockHeader* block) {
    if (block->next)
        return block->next;
    else {

        void* address = allocate_new_page();
        if (!address)
            return 0;

        return new_free_block(address, 4096, block, 0);
    }
}


// Finally the allocation function
void* malloc(uint32_t size) {
    if (active_memory.first_block == 0) {

        // Create first block at new page
        void* address = allocate_new_page();
        if (!address)
            return 0;
        active_memory.first_block = new_free_block(address, 4096, 0, 0);
    }

    if (size == 0)
        return 0;

    // Now an infinite loop to search for an appropriate free block
    struct MemoryBlockHeader* block = active_memory.first_block;
    while (1) {
        if (block == 0) {
            break;
        }
        else if (block->magic == FREE_BLOCK && block->size > size) {

            // Allocate this block
            block->magic = ALLOC_BLOCK;

            // Check if there is enough size for another free block after this block

            if (block->size > size + sizeof(struct MemoryBlockHeader)) {
                new_free_block((void*)((uint32_t)block + sizeof(struct MemoryBlockHeader) + size),
                               block->size - size, block, block->next);
                block->size = size;
            }

            // Return the allocated block
            return (void*)((uint32_t)block + sizeof(struct MemoryBlockHeader));
        }
        else {
            block = next_block(block);
        }
    }

    return 0;
}


// Free the allocated memory
uint32_t free(void* address) {
    struct MemoryBlockHeader* block = (struct MemoryBlockHeader*)((uint32_t)address - sizeof(struct MemoryBlockHeader));
    if (block->magic != ALLOC_BLOCK) {
        return 0;
    }
    else {
        // TODO: Also free the page if needed

        uint32_t size = block->size;
        block->magic = FREE_BLOCK;
        block = merge_blocks(block);
        // return block->size;
        return size;
    }
}