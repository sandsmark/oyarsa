extern "C" {
#include "common.h"
}

#include "vmm.h"
#include "pmm.h"

uint32_t PhysMemManager::m_location;
uint32_t PhysMemManager::m_stackLocation = PMM_STACK_OFFSET;
uint32_t PhysMemManager::m_maxPosition = PMM_STACK_OFFSET;
bool PhysMemManager::pagingActive = false;

void PhysMemManager::initialize(uint32_t start)
{
    // To align we add a page size and mask out the three lower bits
    m_location = (start + PAGE_SIZE) & PAGE_MASK;
}

uint32_t PhysMemManager::allocatePage()
{
    if (pagingActive) {
        if (m_stackLocation == PMM_STACK_OFFSET)
            panic("Out of memory");

        m_stackLocation -= sizeof(uint32_t);
        uint32_t *stack = (uint32_t*)m_stackLocation;
        return *stack;
    } else {
        return m_location += PAGE_SIZE;
    }
}

void PhysMemManager::freePage(uint32_t page)
{
    if (page < m_location)
        return;

    if (m_maxPosition <= m_stackLocation) // are we out of stack space?
    {
        VirtualMemoryManager::map(m_maxPosition, page, PAGE_PRESENT | PAGE_WRITE); // we stealz this freed page
        m_maxPosition += PAGE_SIZE;
    } else {
        uint32_t *stack = (uint32_t*)m_stackLocation;
        *stack = page;
        m_stackLocation += sizeof(uint32_t);
    }
}

