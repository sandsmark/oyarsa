#include "threading.h"
#include "heap.h"
#include "monitor.h"

uint32_t next_tid = 0;

thread_t *current_thread;

thread_t *threading_init()
{
    thread_t *thread = kmalloc(sizeof(thread_t));
    thread->id = next_tid++;
    
    current_thread = thread;
}

void thread_exit()
{
    register uint32_t ret __asm ("eax");

    monitor_write("Thread ended with: ");
    monitor_write_dec(ret);
    monitor_put('\n');
}

thread_t *create_thread(int (*function)(void*), void *arg, uint32_t *stack)
{
    thread_t *thread = kmalloc(sizeof(thread_t));
    memset(thread, 0, sizeof(thread_t));
    thread->id = next_tid++;

    // replace stack stuffs
    *--stack = (uint32_t)arg;
    *--stack = (uint32_t)&thread_exit; // fake return address
    *--stack = (uint32_t)function;
    *--stack = 0; // fake EBP

    thread->ebp = (uint32_t)stack;
    thread->esp = (uint32_t)stack;

    return thread;
}

void switch_thread(thread_t *next)
{
    // store registers for later
    __asm volatile("mov %%esp, %0" : "=r"(current_thread->esp));
    __asm volatile("mov %%ebp, %0" : "=r"(current_thread->ebp));
    __asm volatile("mov %%ebx, %0" : "=r"(current_thread->ebx));
    __asm volatile("mov %%esi, %0" : "=r"(current_thread->esi));
    __asm volatile("mov %%edi, %0" : "=r"(current_thread->edi));
    
    // switch 
    current_thread = next;

    // replace regs
    __asm volatile("mov %0, %%esp" : : "r"(current_thread->esp));
    __asm volatile("mov %0, %%ebp" : : "r"(current_thread->ebp));
    __asm volatile("mov %0, %%ebx" : : "r"(current_thread->ebx));
    __asm volatile("mov %0, %%esi" : : "r"(current_thread->esi));
    __asm volatile("mov %0, %%edi" : : "r"(current_thread->edi));

}

