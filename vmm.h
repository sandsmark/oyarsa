#ifndef VMM_H
#define VMM_H

#include "common.h"
#include "idt.h"

#define PAGE_DIR_VIRTUAL_ADDR 0xFFBFF000
#define PAGE_TABLE_VIRTUAL_ADDR 0xFFC00000

#define PAGE_DIR_IDX(x) ((uint32_t)x/1024)
#define PAGE_TABLE_IDX(x) ((uint32_t)x%1024)

#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4
#define PAGE_MASK 0xFFFFF000
#define PAGE_SIZE 0x1000

typedef uint32_t page_directory_t;

class VirtualMemoryManager {
public:
    enum PageFlags {
        Present = 0x1,
        Writeable = 0x2,
        User = 0x4
    };

    static void initialize();

    /// switches active directory
    static void switchDirectory(page_directory_t *directory);

    /// maps physical memory pa to virtual mem va, with given flags
    static void map(uint32_t va, uint32_t pa, uint32_t flags);

    /// removes a mapping from virtual to physical memory
    static void unmap(uint32_t va);

    /// handler for page faults
    static void pagefaultHandler(registers_t *regs);

    /**
     * returns 1 if virtual address is mapped in address space
     * puts physical address in pa if non-null
     */
    static char getMapping(uint32_t va, uint32_t *pa);

private:
    static uint32_t *m_directory;
    static uint32_t *m_tables;
};

#endif//VMM_H
