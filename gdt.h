#ifndef GDT_H
#define GDT_H

#include "common.h"

struct GdtEntry
{
  uint16_t limit_low;     // The lower 16 bits of the limit.
  uint16_t base_low;      // The lower 16 bits of the base.
  uint8_t base_middle;    // The next 8 bits of the base.
  uint8_t access;         // Access flags, determines what ring this segment can be used in.
  uint8_t granularity;
  uint8_t base_high;      // The last 8 bits of the base.
} __attribute__((packed));

class GdtManager
{
public:
    static void initialize();

private:
    GdtManager ();
    GdtManager (const GdtManager&);
    GdtManager &operator= (const GdtManager&);

    ~GdtManager();

    void setGate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

    size_t m_entries;
    GdtEntry m_gdt[3];
    static GdtManager *m_instance;
};

#endif//GDT_H
