#pragma once

/*
    For each page directory, we maintain a Memory struct.
    Each memory contains a list of Blocks, with Block-Headers.
    Each header contains a magic:
        0xc001d00d == free memory
        0xdeadd00d == allocated memory
        size = total free/allocated memory after this header
        prev = null for first block
        next = null for last block
*/

struct MemoryBlockHeader {
    uint32_t magic;
    uint32_t size;
    struct MemoryBlockHeader* next;
    struct MemoryBlockHeader* prev;
};

struct Memory {
    // 4GB memory = 1024*1024 pages: this will take up 128KB of memory itself :O
    uint32_t pages[1024*1024/32];

    struct MemoryBlockHeader* first_block;
};

extern struct Memory active_memory;

void* malloc(uint32_t size);
uint32_t free(void* address);