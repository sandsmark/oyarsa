#include "common.h"
#include "multiboot.h"
#include "monitor.h"
#include "math.h"
#include "gdt.h"
#include "idt.h"
#include "effects.h"

int kernel_main(multiboot_t *mboot)
{
    gdt_init();
    idt_init();
    __asm volatile("int $0x3");

//    monitor_clear();
//    monitor_write("Hello world!");

//    effects_run();

    return 0xdeadbeef;
}
