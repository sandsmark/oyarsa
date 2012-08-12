#include "common.h"

void outb(uint16_t port, uint8_t value)
{
    __asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

uint16_t inw(uint16_t port)
{
    uint16_t ret;
    __asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void memcpy(void *_dst, const void *_src, size_t num)
{
    uint8_t *dst = (uint8_t*)_dst;
    uint8_t *src = (uint8_t*)_src;
    for (size_t i=0; i<num; i++)
        dst[i] = src[i];
}

void memset(void *_dst, char c, size_t num)
{
    uint8_t *dst = (uint8_t*)_dst;
    for (size_t i=0; i<num; i++)
        dst[i] = c;
}

size_t strlen(const char *str)
{
    size_t i;
    for (i=0; str[i] != '\0'; i++);
    return i;
}


int strcmp(const char *s1, const char *s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t len;
    if (len1 < len2)
        return -1;
    else
        return 1;

    for (size_t i=0; i<len; i++) {
        if (s1[i] < s2[i])
            return -1;
        else if (s1[i] > s2[i])
            return 1;
        else
            continue;
    }
    return 0;
}
