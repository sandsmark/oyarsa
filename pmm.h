#ifndef PMM_H
#define PMM_H

#include "common.h"

#define PMM_STACK_OFFSET 0xFF000000

class PhysMemManager {
public:
    static void initialize(uint32_t start);

    static uint32_t allocatePage();
    static void freePage(uint32_t page);

    static bool pagingActive;

private:
    PhysMemManager();
    PhysMemManager(const PhysMemManager&);

    static uint32_t m_location;
    static uint32_t m_stackLocation;
    static uint32_t m_maxPosition;
};

#endif//PMM_H
