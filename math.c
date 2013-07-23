#include "math.h"

float sin(float n)
{
    float ret;
    __asm volatile("fsin" : "=t" (ret) : "0" (n) : "0");
    return ret;
}

float cos(float n)
{
    float ret;
    __asm volatile("fcos" : "=t" (ret) : "0" (n) : "0");
    return ret;
}

float sqrt(float n)
{
    float ret;
    __asm volatile("fsqrt" : "=t" (ret) : "0" (n) : "0");
    return ret;
}

float fabs(float n)
{
    float ret;
    __asm volatile("fabs" : "=t" (ret) : "0" (n) : "0");
    return ret;
}

float distance(float x1, float y1, float x2, float y2)
{
    float x = fabs(x2-x1);
    float y = fabs(y2-y1);
    return sqrt(x+y);
}

