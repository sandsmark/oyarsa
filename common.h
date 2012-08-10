#ifndef COMMON_H
#define COMMON_H

typedef unsigned int uint32_t;
typedef int int32_t;

typedef unsigned short uint16_t;
typedef short int16_t;

typedef unsigned char uint8_t;
typedef char int8_t;

typedef uint32_t size_t;

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

void memcpy(uint8_t *dest, const uint8_t *src, size_t num);
void memset(uint8_t *dst, char c, size_t num);
size_t strlen(const char *str);
int strcmp(const char *s1, const char *s2);

#endif//COMMON_H

