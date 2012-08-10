#include "monitor.h"

uint16_t *video_memory = (uint16_t*)0xB8000;

static uint8_t cursor_y = 0, cursor_x = 0;

static void move_cursor()
{
    uint16_t location = cursor_y * 80 + cursor_x;
    outb(0x3d4, 14); // we are setting the high cursor byte
    outb(0x3d5, location >> 8); // bitshift so we just get the high part of the nibble

    outb(0x3d4, 15); // and now the low byte
    outb(0x3d5, location);
}

static void scroll()
{
    uint8_t attribute = (0 /* black*/ << 4) | (15 /* white */ & 0x0f);
    uint16_t blank = 0x20 /* space */ | (attribute << 8);

    if (cursor_y >= 25)
    {
        for (int i=0; i<24*80; i++)
            video_memory[i] = video_memory[i+80];

        for (int i=24*80; i<25*80; i++)
            video_memory[i] = blank;
        cursor_y = 24;
    }
}

void monitor_put(char c)
{
    monitor_put_styled(c, 15, 0);
}

void monitor_put_styled(char c, int fg, int bg)
{
    uint8_t colorByte = (bg << 4) | (fg & 0x0f);
    uint16_t attribute = colorByte << 8;

    // handle backspace
    if (c == 0x08 && cursor_x != 0) {
        cursor_x--;

    // handle tab, increasing to nearest divisible by 8 (we zero out the lowermost zero bits?)
    } else if (c == 0x09) {
        cursor_x = (cursor_x+8) & ~(8-1);

    // handle return
    } else if (c == '\r' || c == '\n') {
        cursor_x = 0;
        cursor_y++;

    } else if ( c >= ' ') {
        video_memory[cursor_y*80 + cursor_x] = c | attribute;
        cursor_x++;
    }

    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();
    move_cursor();
}

void monitor_clear()
{
    uint8_t attribute = (0 /*black*/ << 4) | (15 /*white*/ & 0x0f);
    uint16_t blank = 0x20/*space*/ | (attribute<<8);

    for (int i=0; i<80*25; i++) {
        video_memory[i] = blank;
    }

    cursor_x=0;
    cursor_y=0;
    move_cursor();
}

void monitor_write(const char *string)
{
    for (int i=0; string[i] != '\0'; i++) {
        monitor_put(string[i]);
    }
}

static char num_to_hex(uint32_t b)
{
    char r;
    b &= 0x000f; // we only want the lower 4 bits
    if (b > 9) {
        r = b - 10 + 'A';
    } else {
        r = b + '0';
    }
}
void monitor_write_hex(uint32_t n)
{
    monitor_write("0x");

    monitor_put(num_to_hex(n>>28));
    monitor_put(num_to_hex(n>>24));
    monitor_put(num_to_hex(n>>20));
    monitor_put(num_to_hex(n>>16));
    monitor_put(num_to_hex(n>>12));
    monitor_put(num_to_hex(n>>8));
    monitor_put(num_to_hex(n>>4));
    monitor_put(num_to_hex(n));
}

void monitor_write_dec(uint32_t n)
{
    char buf[11] = {0}; // Can never be more than 11 digits (log10(2^32) = 9.63, and we also reserve 1 for \0)
    char *cur = buf + 10;
    while (n > 0) {
        cur--;
        *cur = n%10 + '0';
        n /= 10;
    }
    monitor_write(cur);
}
