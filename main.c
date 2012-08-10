#include "common.h"
#include "multiboot.h"
#include "monitor.h"
#include "math.h"
int kernel_main(multiboot_t *mboot)
{
    monitor_clear();
    monitor_write("Hello world!");
    monitor_put(' ');
    monitor_write_hex(0xdeadbeef);
    monitor_put(' ');
    monitor_write_dec(65535);
    monitor_put('\n');

    for (int x=0; x<80; x++) {
        for (int y=0; y<23; y++) {
            monitor_put_styled(' ', 0, cos((x+y)/100.0)*15);
        }
    }

    return 0xdeadbeef;
}
