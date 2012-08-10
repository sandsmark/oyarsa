#include "math.h"

float sin(float n)
{
    float ret;
    __asm volatile("fsin" : "=t" (ret) : "0" (n));
    return ret;
}

float cos(float n)
{
    float ret;
    __asm volatile("fcos" : "=t" (ret) : "0" (n));
    return ret;
}
