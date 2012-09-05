#include "heap.h"

#include "pmm.h"
#include "vmm.h"

uint32_t heap_max = HEAP_START;
header_t *heap_head = 0;

void *kmalloc(size_t len)
{
    len += sizeof(header_t);

    header_t *cur = heap_head, *prev = 0;
    while (cur) {
        if (cur->allocated == 0 && cur->length >= len) {
            // split chunk, if possible
            if (cur->length - len > sizeof(header_t)) { // header + 1 byte is minimum to be useful
                header_t *newchunk = (header_t*)((uint32_t)cur + cur->length);
                newchunk->prev = cur;
                newchunk->next = cur->next; //TODO: check this
                newchunk->allocated = 0;
                newchunk->length = cur->length - len;

                cur->next = newchunk;
                cur->length = len;
            }

            cur->allocated = 1;
            return (void*)((uint32_t)cur + sizeof(header_t));
        }
        prev = cur;
        cur = cur->next;
    }

    uint32_t chunk_start;
    if (prev) {
        chunk_start = (uint32_t)prev + prev->length;
    } else { // first chunk
        chunk_start = HEAP_START;
        heap_head = (header_t*)chunk_start;
    }

    // Grow our heap until we fit
    while (chunk_start + len > heap_max) {
        uint32_t page = pmm_alloc_page();
        map(heap_max, page, PAGE_PRESENT | PAGE_WRITE);
        heap_max += PAGE_SIZE;
    }

    cur = (header_t*)chunk_start;
    cur->prev = prev;
    cur->next = 0;
    cur->allocated=1;
    cur->length = len;

    prev->next = cur;

    return (void*)(chunk_start + sizeof(header_t)); // hide our header
}

void kfree(void *p)
{
    header_t *header = (header_t*)((uint32_t)p - sizeof(header_t)); // find our header again
    header->allocated = 0;

    if (header->next && header->next->allocated == 0) {
        header->length += header->next->length;
        header->next = header->next->next;
    }

    if (header->prev && header->prev->allocated == 0) {
        header->length += header->prev->length;
        header->prev = header->prev->prev;
    }

    if (header->next == 0) { // We are at the end of the heap, time to contract
        if (header->prev == 0)
            heap_head = 0;
        else
            header->prev->next = 0;

        while ((heap_max - PAGE_SIZE) >= (uint32_t)header) {
            heap_max -= PAGE_SIZE;
            uint32_t page;
            get_mapping(heap_max, &page);
            pmm_free_page(page);
            unmap(heap_max);
        }
    }
}

