extern "C" {
#include "common.h"
#include "multiboot.h"
#include "monitor.h"
#include "math.h"
#include "gdt.h"
#include "idt.h"
#include "effects.h"
#include "timer.h"
#include "elf.h"
#include "pmm.h"
#include "vmm.h"
#include "scheduler.h"
}
elf_t kernel_elf;

extern "C" int kernel_main(multiboot_t *mboot)
{
    monitor_clear();
    monitor_write("oyarsa 0.1\n");

    monitor_write("Initializing global descriptor table...\n");
    gdt_init();
    monitor_write("Initializing interrupt table...\n");
    idt_init();
    monitor_write("Initializing timer...\n");
    timer_init(20);
    monitor_write("Initializing physical memory...\n");
    pmm_init(mboot->mem_upper);
    monitor_write("Initializing virtual memory...\n");
    vmm_init();
    monitor_write("Parsing ELF...\n");
    kernel_elf = elf_from_multiboot(mboot);

    monitor_write("Looking for memory... ");
    // find free memory
    uint32_t ptr = mboot->mmap_addr;
    uint32_t amount = 0;
    while (ptr < mboot->mmap_addr + mboot->mmap_length) {
        mmap_entry_t *map = (mmap_entry_t*)ptr;

        if (map->type == 1) { // type == RAM
            for (uint32_t page = map->base_addr_low; page < map->base_addr_low + map->length_low; page += PAGE_SIZE) {
                pmm_free_page(page);
                amount++;
            }
        }

        ptr += map->size + sizeof(uint32_t);
    }
    monitor_write_dec(amount / 256);
    monitor_write("MB free!\n");


    monitor_write("Initializing scheduler...\n");
    scheduler_init(threading_init());


    __asm volatile("sti");

    panic("Testing panic button! :D");

    return 0xdeadbeef;
}
