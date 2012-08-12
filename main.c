#include "common.h"
#include "multiboot.h"
#include "monitor.h"
#include "math.h"
#include "gdt.h"
#include "idt.h"
#include "effects.h"
#include "timer.h"

int kernel_main(multiboot_t *mboot)
{
    monitor_clear();
    monitor_write("oyarsa 0.1\n");

    gdt_init();
    idt_init();
    timer_init(20);
    __asm volatile("sti");


    effects_run();

    return 0xdeadbeef;
}
