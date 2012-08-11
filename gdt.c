
#include "common.h"
#include "gdt.h"

// in gdt_s.s
extern void gdt_flush (uint32_t);

// The GDT itself.
gdt_entry_t gdt_entries [3];

// Pointer structure to give to the CPU.
gdt_ptr_t gdt_ptr;

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt_entries[num].base_low = (base & 0xffff);
    gdt_entries[num].base_middle = (base >> 16) & 0xff;
    gdt_entries[num].base_high = (base >> 24) & 0xff;

    gdt_entries[num].limit_low = (limit & 0xffff);
    gdt_entries[num].granularity = (limit >> 16) & 0x0f;
    gdt_entries[num].granularity |= granularity & 0xf0;

    gdt_entries[num].access = access;
}

void gdt_init()
{
    gdt_ptr.base = (uint32_t)&gdt_entries; // pointer to our structure
    gdt_ptr.limit = 3*sizeof(gdt_entry_t) - 1; // last valid byte is size - 1

    gdt_set_gate(0, 0, 0, 0, 0); // null segment
    gdt_set_gate(1, 0, 0xfffff, 0x9a, 0xcf); // code segment (access: segment present: 1, dpl (ring): 00, descriptor type: 1, type: 1010)
    gdt_set_gate(2, 0, 0xfffff, 0x92, 0xcf); // data segment (access: segment present: 1, dpl (ring): 00, descriptor type: 1, type: 0010)

    // push to CPU
    gdt_flush((uint32_t) &gdt_ptr);
}

