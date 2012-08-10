#include "common.h"
#include "multiboot.h"
#include "monitor.h"
int kernel_main(multiboot_t *mboot)
{
    monitor_clear();
    monitor_write("Hello world!");
    monitor_put(' ');
    monitor_write_hex(0xdeadbeef);
    monitor_put(' ');
    monitor_write_dec(65535);
    monitor_put('\n');

    return 0xdeadbeef;
}
