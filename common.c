#include "common.h"
#include "monitor.h"
#include "elf.h"

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
/*for (size_t i=0; i<num; i++)
        dst[i] = c;*/
    monitor_write("zeroing..\n");
    for (;num!=0;num--) *dst++=c;
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
    if (len1 < len2)
        return -1;
    else if (len1 > len2)
        return 1;

    size_t len = len1;
    for (size_t i=0; i<len; i++) {
        char diff = s1[i] - s2[i];
        if (diff != 0)
            return diff;
    }

    return 0;
}

extern elf_t kernel_elf;

void print_stacktrace()
{
    uint32_t *ebp, *eip;
    __asm volatile("mov %%ebp, %0" : "=r" (ebp)); // get current EBP

    while (ebp) { // we zero this out in boot.s
        eip = ebp+1;
        monitor_write("\t[");
        monitor_write_hex(*eip);
        monitor_write("] ");
        monitor_write(elf_lookup_symbol(*eip, &kernel_elf));
        monitor_put('\n');
        ebp = (uint32_t*) *ebp;
    }
}

void panic(const char *msg)
{
    monitor_write(" *** PANIC: ");
    monitor_write(msg);
    monitor_put('\n');
    print_stacktrace();
    monitor_write(" ***\n");

    for (;;);
}
