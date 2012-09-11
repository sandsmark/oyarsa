#include "scheduler.h"
#include "common.h"
#include "heap.h"

thread_list_t *thread_list; // circular list

void scheduler_init(thread_t *initial_thread)
{
    thread_list = new thread_list_t; // kmalloc(sizeof(thread_list_t));
    thread_list->thread = initial_thread;
    thread_list->next = thread_list;
}

void thread_is_ready(thread_t *t)
{
    thread_list_t *item = thread_list;
    while (item->next)
        item = item->next;

    thread_list_t *n = new thread_list_t; //kmalloc(sizeof(thread_list_t));
    n->thread = t;
    n->next = thread_list;

    item->next = n;
}

void thread_is_not_ready(thread_t *t)
{
    if (thread_list->thread == t) { // head of list
        thread_list_t *tmplist = thread_list;
        thread_list = thread_list->next;
        kfree(tmplist);
        return;
    }

    for (thread_list_t *item = thread_list->next, *prev = thread_list;
            item != thread_list;
            item = item->next, prev = prev->next) {
        if (item->thread == t) {
            prev->next = item->next; 
            kfree(item);
            return;
        }
    }
}

void schedule()
{
    if (!thread_list)
        return;

    thread_list = thread_list->next;
    switch_thread(thread_list->thread);
}


