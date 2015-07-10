#include "sound.h"

#define PORT_COMMAND 0x43
#define PORT_DATA1 0x40
#define PORT_DATA2 0x41
#define PORT_DATA3 0x42

#define PORT_KEYBOARD_CTRL 0x61

#define PIT_FREQUENCY 1193180

void sound_play(uint32_t freq)
{
    uint32_t divisor = PIT_FREQUENCY / freq;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    // first two bits = 10 = timer 2
    // second two bits = 11 = lsb first
    // next three bits = 011 = square wave generator
    // last bit = 0 = 16 bit binary counter
    // 10 11 011 0 = 0xB6
    outb(PORT_COMMAND, 0xB6);

    outb(PORT_DATA1, low);
    outb(PORT_DATA1, high);
    
    // Ensure that the PIT is connected to the speaker
    uint8_t oldval = inb(PORT_KEYBOARD_CTRL);
//    if (!(oldval & 0x3)) {
        outb(PORT_KEYBOARD_CTRL, oldval | 0x3);
//    }
}

void sound_stop()
{
    // Disconnect the PIT from the speaker
    uint8_t oldval = inb(PORT_KEYBOARD_CTRL);
    outb(PORT_KEYBOARD_CTRL, oldval & 0xFC); // clear bit 0
}
