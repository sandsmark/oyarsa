#include "vmm.h"
#include "pmm.h"
#include "monitor.h"

uint32_t pmm_location;
uint32_t pmm_stack_loc = PMM_STACK_OFFSET;
uint32_t pmm_stack_max = PMM_STACK_OFFSET;

char pmm_paging_active = 0;

void pmm_init(uint32_t start)
{
    // To align we add a page size and mask out the three lower bits
    pmm_location = (start + PAGE_SIZE) & PAGE_MASK;
}

uint32_t pmm_alloc_page()
{
    if (pmm_paging_active) {
        if (pmm_stack_loc == PMM_STACK_OFFSET)
            panic("Out of memory");

        pmm_stack_loc -= sizeof(uint32_t);
        uint32_t *stack = (uint32_t*)pmm_stack_loc;
        return *stack;
    } else {
        return pmm_location += PAGE_SIZE;
    }
}

void pmm_free_page(uint32_t page)
{
    if (page < pmm_location)
        return;

    if (pmm_stack_max <= pmm_stack_loc) // are we out of stack space?
    {
        map(pmm_stack_max, page, PAGE_PRESENT | PAGE_WRITE); // we stealz this freed page
        pmm_stack_max += PAGE_SIZE;
    } else {
        uint32_t *stack = (uint32_t*)pmm_stack_loc;
        *stack = page;
        pmm_stack_loc += sizeof(uint32_t);
    }
}

