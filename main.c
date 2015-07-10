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

elf_t kernel_elf;
extern void (*timer_handler)(uint32_t tick);

const char string [] = "greets to indigo/zomgtronics/bitflavour. code:martin,gfx:noone,msx:a deaf guy.";
int colors[] = { 7, 8, 15 };


float g(float x, float y, float t)
{
//    return sin(x/40.0 +t);
    float px = 40.0f * sin(-t) + 40.0f;
    float py = 12.0f * cos(-t)*sin(-t/2) + 12.0f;
    float v1 = sin(distance(x, y, px, py));

    float v2 = sin(sin(t)*x/(37+15*cos(y))) * cos(y/(31 +11*sin(x)));
    return (v1 + v2) /2;
    //return (sin(x/16.0) + sin(y/32.0) + sin(distance(x, t*y, 80, 12)/8.0) + sin(sqrt(x*x+y*y))/8.0) / 4.0;
}

float f(float x, float y, float t)
{
//    return sin(x/40.0 +t);
    //return sin(distance(x, y, (40 * sin(-t) + 40), (12 * cos(-t) + 12)) / 40.74);
    return (sin(x/16.0) + sin(y/32.0) + sin(distance(x, t*y, 80, 12)/8.0) + sin(sqrt(x*x+y*y))/8.0) / 4.0;
}

void doanim(uint32_t tick)
{
    int length = strlen(string);
    monitor_move_cursor(0, 0);
    for (int i=0; i<79; i++) {
        int color = colors[(i/10) % 3];
        const int textt = ((tick/3) % 80);
        int offset = i - textt;
        if (offset >= 0 && offset < length) {
            monitor_put_styled(string[offset], color, 0);
        } else if (i < (textt + length) % 80 && textt + length > 80) {
            monitor_put_styled(string[length - (((textt + length) % 80) - i)], color, 0);
        } else {
            monitor_put(' ');
        }
    }
    monitor_put('\n');
    for (int y=0; y<22; y++) {
        for (int x=0; x<80; x++) {
            monitor_put_styled(' ', 0, (int)(g(x, y, (float)tick/20.0)*15) % 15);
            //monitor_put_styled(' ', 0, colors[(int)(f(x, y, (float)t/20.0)*3)]);
        }
    }
}

void dowait(uint32_t tick)
{
    static uint32_t firsttick = 0;
    if (!firsttick) {
        firsttick = tick;
    }
    if (tick - firsttick > 10) {
        timer_handler = &doanim;
    }
}

int kernel_main(multiboot_t *mboot)
{
    monitor_clear();
    monitor_write("oyarsa 0.1\n");

    monitor_write("Initializing global descriptor table...\n");
    gdt_init();
    monitor_write("Initializing interrupt table...\n");
    idt_init();
    monitor_write("Initializing timer...\n");
    timer_init(10);
    monitor_write("Initializing physical memory...\n");
    pmm_init(mboot->mem_upper);
    monitor_write("Initializing virtual memory...\n");
    vmm_init();
    monitor_write("Parsing kernel ELF...\n");
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
    monitor_write_dec(amount * 4 / 1024);
    monitor_write("MB found!\n");

    monitor_write("Initializing scheduler...\n");
    scheduler_init(threading_init());

    monitor_write("Enabling interrupts...\n");
    __asm volatile("sti");

    monitor_write("=============\nBoot complete.\n============\n");
    monitor_write("Calculating hard...\n");

//    effects_run();
    timer_handler = &dowait;
    while (1) { }

    panic("Testing panic button! Backtrace follows:");

    return 0xdeadbeef;
}
