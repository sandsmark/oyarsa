#ifndef HEAP_H
#define HEAP_H

#include "common.h"

#define HEAP_START 0xD0000000
#define HEAP_END 0xFFBFF000

typedef struct header
{
    struct header *prev, *next;
    uint32_t allocated : 1;
    uint32_t length: 31;
} header_t;

void heap_init();
void *kmalloc(size_t len);
void kfree(void *p);

#ifdef __cplusplus
void *operator new(size_t len) { return kmalloc(len); }
void *operator new[](size_t len) { return kmalloc(len); }
void operator delete(void *p) { kfree(p); }
void operator delete[](void *p) { kfree(p); }
#endif//__cplusplus

#endif//HEAP_H

