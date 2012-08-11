#include "common.h"
#include "multiboot.h"
#include "monitor.h"
#include "math.h"
#include "gdt.h"
#include "effects.h"

int kernel_main(multiboot_t *mboot)
{
    gdt_init();

    monitor_clear();
    monitor_write("Hello world!");

    effects_run();

    return 0xdeadbeef;
}
