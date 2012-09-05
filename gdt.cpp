#include "common.h"
#include "gdt.h"

extern "C" {
#include "heap.h"
}

// from gdt_s.s
extern "C" void gdt_flush (uint32_t);

// The GDT object is
GdtManager *GdtManager::m_instance = 0;

// Used when passing the pointer to our GDT
struct GdtPointer
{
    uint16_t entries;
    uint32_t pointer;
} __attribute__((packed));

// The GDT itself.
GdtEntry gdt_entries [3];

// Pointer structure to give to the CPU.
GdtPointer gdt_ptr;

void GdtManager::initialize()
{
    if (m_instance) // We are already initialized
        return;

    m_instance = new GdtManager();
}

GdtManager::GdtManager()
{
    gdt_ptr.pointer = (uint32_t)&m_gdt; // pointer to our structure
    gdt_ptr.entries = 3*sizeof(m_gdt) - 1; // last valid byte is size - 1

    setGate(0, 0, 0, 0, 0); // null segment
    setGate(1, 0, 0xfffff, 0x9a, 0xcf); // code segment (access: segment present: 1, dpl (ring): 00, descriptor type: 1, type: 1010)
    setGate(2, 0, 0xfffff, 0x92, 0xcf); // data segment (access: segment present: 1, dpl (ring): 00, descriptor type: 1, type: 0010)

    // push to CPU
    gdt_flush((uint32_t) &gdt_ptr);
}


void GdtManager::setGate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    m_gdt[num].base_low = (base & 0xffff);
    m_gdt[num].base_middle = (base >> 16) & 0xff;
    m_gdt[num].base_high = (base >> 24) & 0xff;

    m_gdt[num].limit_low = (limit & 0xffff);
    m_gdt[num].granularity = (limit >> 16) & 0x0f;
    m_gdt[num].granularity |= granularity & 0xf0;

    m_gdt[num].access = access;
}

