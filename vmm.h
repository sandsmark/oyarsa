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

// change address space
void switch_page_directory(page_directory_t *pd);

// maps physical memory pa to virtual mem va, with given flags
void map(uint32_t va, uint32_t pa, uint32_t flags);

void unmap(uint32_t va);

/// returns 1 if virtual address is mapped in address space
/// puts physical address in pa if non-null
char get_mapping(uint32_t va, uint32_t *pa);

// handler for page faults
void page_fault(registers_t *regs);

void vmm_init();


#endif//VMM_H
