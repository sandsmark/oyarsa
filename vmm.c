#include "idt.h"
#include "vmm.h"
#include "pmm.h"
#include "monitor.h"

uint32_t *page_directory = (uint32_t*)PAGE_DIR_VIRTUAL_ADDR;
uint32_t *page_tables = (uint32_t*)PAGE_TABLE_VIRTUAL_ADDR;

page_directory_t *current_directory;

extern char pmm_paging_active;

void vmm_init()
{
    idt_register_interrupt_handler(14, &page_fault);

    // allocate a page directory
    page_directory_t *pd = (page_directory_t*)pmm_alloc_page();

    // zero it out
    memset(pd, 0, PAGE_SIZE);

    pd[0] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;
    uint32_t *pt = (uint32_t*)(pd[0] & PAGE_MASK);
    for (int i=0; i<1024; i++) {
        pt[i] = i*PAGE_SIZE | PAGE_PRESENT | PAGE_WRITE;
    }

    // assign second last table and zero out
    pd[1022] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;
    pt = (uint32_t*)(pd[1022] & PAGE_MASK);
    memset(pt, 0, PAGE_SIZE);

    // http://wiki.osdev.org/Paging#Manipulation
    //
    // The last entry of the second-last table is the directory itself.
    pt[1023] = (uint32_t)pd | PAGE_PRESENT | PAGE_WRITE;
    // ..and the last table loops back on the directory
    pd[1023] = (uint32_t)pd | PAGE_PRESENT | PAGE_WRITE;


    switch_page_directory(pd);

    // turn on paging
    uint32_t cr0;
    __asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm volatile("mov %0, %%cr0" : : "r"(cr0));

    uint32_t pt_idx = PAGE_DIR_IDX((PMM_STACK_OFFSET>>12));
    page_directory[pt_idx] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;
    memset((uint32_t*)page_tables[pt_idx*1024], 0, PAGE_SIZE);

    pmm_paging_active=1;
}

void switch_page_directory(page_directory_t *pd)
{
    current_directory = pd;
    __asm volatile("mov %0, %%cr3" : : "r"(pd));
}

void map(uint32_t va, uint32_t pa, uint32_t flags)
{
    uint32_t virtual_page = va / PAGE_SIZE;
    uint32_t pt_idx = PAGE_DIR_IDX(virtual_page);

    if (page_directory[pt_idx] == 0) {
        // create page
        page_directory[pt_idx] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;
        memset((uint32_t*)page_tables[pt_idx*PAGE_SIZE], 0, PAGE_SIZE);
    }

    page_tables[virtual_page] = (pa & PAGE_MASK) | flags;
}

void unmap(uint32_t va)
{
    uint32_t virtual_page = va / PAGE_SIZE;
    page_tables[virtual_page] = 0;

    // tell the CPU that we invalidated it, clears the entry in the TLB
    __asm volatile("invlpg (%0)" : : "a"(va));
}

char get_mapping(uint32_t va, uint32_t *pa)
{
    uint32_t virtual_page = va/PAGE_SIZE;
    uint32_t pt_idx = PAGE_DIR_IDX(virtual_page);

    if (page_directory[pt_idx] == 0) {
        return 0;//return false
    }

    if (page_tables[virtual_page] != 0) {
        if (pa)
            *pa = page_tables[virtual_page] & PAGE_MASK;
        return 1;
    }
}

void page_fault(registers_t *regs)
{
    uint32_t cr2;
    __asm volatile("mov %%cr2, %0" : "=r"(cr2));
    monitor_write("Page fault at ");
    monitor_write_hex(regs->eip);
    monitor_write(", faulting address ");
    monitor_write_hex(cr2);
    monitor_put('\n');

    monitor_write("Error: ");
    switch(regs->err_code) {
    case 0:
        monitor_write("Supervisory process tried to read a non-present page entry\n");
        break;
    case 1:
        monitor_write("Supervisory process tried to read a page and caused a protection fault\n");
        break;
    case 2:
        monitor_write("Supervisory process tried to write to a non-present page entry\n");
        break;
    case 3:
        monitor_write("Supervisory process tried to write a page and caused a protection fault\n");
        break;
    case 4:
        monitor_write("User process tried to read a non-present page entry\n");
        break;
    case 5:
        monitor_write("User process tried to read a page and caused a protection fault\n");
        break;
    case 6:
        monitor_write("User process tried to write to a non-present page entry\n");
        break;
    case 7:
        monitor_write("User process tried to write a page and caused a protection fault\n");
        break;
    default:
        monitor_write("Unknown error\n");
    }

    panic("");
}
