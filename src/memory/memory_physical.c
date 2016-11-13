#include <string.h>
#include <memory.h>

/* Simple physical memory management with bitmaps */

// First a frame-by-frame bitmap

static uint32_t num_frames;
static uint32_t used_frames;
static uint32_t * frames;

static inline void set_frame(uint32_t bit) {
    frames[bit/32] |= (1 << (bit % 32));
}

static inline void unset_frame(uint32_t bit) {
    frames[bit/32] &= ~(1 << (bit % 32));
}

static inline int test_frame(uint32_t bit) {
    return frames[bit/32] & (1 << (bit % 32));
}

static int get_first_free_frame() {
    uint32_t i, j;
    for (i=0; i<num_frames/32; ++i) {
        if (frames[i] != 0xFFFFFFFF) {
            for (j=0; j<32; ++j) {
                if (!(frames[i] & (1 << j))) {
                    return i*32+j;
                }
            }
        }
    }
    return -1;
}

// Next the actual physical memory management code

void pm_init(uint32_t mem_size_in_kb, uint32_t* bitmap_storage_address)
{
    used_frames = num_frames = mem_size_in_kb / 4;	// each frame is 4KB
    frames = bitmap_storage_address;
    memset((char*)frames, 0xFF, sizeof(uint32_t) * num_frames / 8);	// mark as all used
}

void pm_init_region(uint32_t base_address, uint32_t size)
{
    unsigned a, b;
    for (a=base_address/4096, b=size/4096; b>0; --b) {
        unset_frame(a++);
        used_frames++;
    }
    set_frame(0); 													// 0x0 address is always invalid
}

void pm_deinit_region(uint32_t base_address, uint32_t size)
{
    unsigned a, b;
    for (a=base_address/4096, b=size/4096; b>0; --b) {
        set_frame(a++);
        used_frames--;
    }
}

void* pm_alloc_frame()
{
    if (used_frames == num_frames)
        return 0;
    int frame = get_first_free_frame();
    if (frame == -1)
        return 0;
    set_frame(frame);
    used_frames++;

    return (void*)(frame*4096);
}

void pm_free_frame(void* address)
{
    uint32_t frame = (uint32_t)address / 4096;
    if (!test_frame(frame))
        return;
    unset_frame(frame);
    used_frames--;
}