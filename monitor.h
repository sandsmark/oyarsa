#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

/// puts a single character on screen
void monitor_put(char c);
void monitor_put_styled(char c, int fg, int bg);

void monitor_move_cursor(uint8_t x, uint8_t y);

/// clears screen
void monitor_clear();

/// Output a string to screen
void monitor_write(const char *);

/// Output a hex value to the screen
void monitor_write_hex(uint32_t n);

/// Output a decimal value to the monitor
void monitor_write_dec(uint32_t n);

#endif//MONITOR_H

