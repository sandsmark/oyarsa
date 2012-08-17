#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "threading.h"

typedef struct thread_list
{
    thread_t *thread;
    struct thread_list *next;
} thread_list_t;

void scheduler_init(thread_t *initial_thread);
void thread_is_ready(thread_t *t);
void thread_is_not_ready(thread_t *t);
void schedule();

#endif//SCHEDULER_H

