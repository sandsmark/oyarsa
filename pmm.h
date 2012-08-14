#ifndef PMM_H
#define PMM_H

#include "common.h"

#define PMM_STACK_OFFSET 0xFF000000


// get the address of a physical page
uint32_t pmm_alloc_page();

// free a page
void pmm_free_page(uint32_t p);

// are we initialized?
extern char pmm_paging_active;

// initialize the physical memory manager
void pmm_init(uint32_t start);

#endif//PMM_H
