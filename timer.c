#include "common.h"
#include "monitor.h"
#include "timer.h"
#include "idt.h"

uint32_t tick = 0;

#define PORT_COMMAND 0x43
#define PORT_DATA1 0x40
#define PORT_DATA2 0x41
#define PORT_DATA3 0x42

#define MODE_REPEATING 0x36

static void timer_callback(registers_t *regs)
{
    tick++;

    monitor_move_cursor(62, 0);
    monitor_write("Timer tick: ");
    monitor_write_dec(tick);
    monitor_put('\n');
}

void timer_init(uint32_t freq)
{
    idt_register_interrupt_handler(IRQ0, &timer_callback);
    outb(PORT_COMMAND, MODE_REPEATING);

    // must fit inside 16 bits
    uint32_t divisor = 1193180/freq;

    // split the divisor into lower/higher parts, it is sent as nibbles
    uint8_t low = (uint8_t)(divisor & 0xff);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xff);

    outb(PORT_DATA1, low);
    outb(PORT_DATA1, high);
}
