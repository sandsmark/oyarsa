#include "common.h"
#include "monitor.h"
#include "timer.h"
#include "idt.h"

uint32_t tick = 0;

#define PORT_SELECT_REG 0x70
#define PORT_CMOS_CONF 0x71

#define REGISTER_A 0x0A
#define REGISTER_B 0x0B
#define REGISTER_C 0x0C

#define DISABLE_NMI 0x80
#define ENABLE_RTC_IRQ 0x40

void (*timer_handler)(uint32_t tick) = 0;

static void timer_callback(registers_t *regs)
{
    // We need to read register C to ensure that this is called again
    outb(PORT_SELECT_REG, REGISTER_C);
    inb(PORT_CMOS_CONF); // we don't care about the contents

    if (timer_handler) {
        timer_handler(tick);
        tick++;
    }
}

void timer_init(uint8_t rate)
{
    // Register our interrupt handler
    idt_register_interrupt_handler(IRQ8, &timer_callback); // RTC

    outb(PORT_SELECT_REG, DISABLE_NMI | REGISTER_B); // select register B

    // Get old value
    uint8_t oldval = inb(PORT_CMOS_CONF);

    outb(PORT_SELECT_REG, DISABLE_NMI | REGISTER_B); // select register B, reading resets to D

    // Actually enable the interrupt without overwriting the existing value
    outb(PORT_CMOS_CONF, oldval | ENABLE_RTC_IRQ);

    // Now to set the frequency
    outb(PORT_SELECT_REG, REGISTER_A);
    oldval = inb(PORT_CMOS_CONF);
    outb(PORT_SELECT_REG, REGISTER_A); // re-set to register a, stupid reads resetting to D
    outb(PORT_CMOS_CONF, oldval | rate);
}
