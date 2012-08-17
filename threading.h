#ifndef THREADING_H
#define THREADING_H

#include "common.h"

typedef struct
{
    // id
    uint32_t id;
    // registers
    uint32_t esp, ebp, esi, edi, eflags, ebx;

} thread_t;

thread_t *threading_init();
thread_t *create_thread(int (*function)(void*), void *arg, uint32_t *stack);
void switch_thread(thread_t *next);

#endif//THREADING_H
