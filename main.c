#include "common.h"
#include "multiboot.h"
#include "monitor.h"
#include "math.h"

float f(float x, float y, float t)
{
//    return sin(x/40.0 +t);
    //return sin(distance(x, y, (40 * sin(-t) + 40), (12 * cos(-t) + 12)) / 40.74);
    return (sin(x/16.0) + sin(y/32.0) + sin(distance(x, t*y, 80, 12)/8.0) + sin(sqrt(x*x+y*y))/8.0) / 4.0;
}

int colors[] = { 0, 8, 7, 10 };


float f(float x, float y, float t)
{
//    return sin(x/40.0 +t);
    float px = 40.0f * sin(-t) + 40.0f;
    float py = 12.0f * cos(-t)*sin(-t/2) + 12.0f;
    float v1 = sin(distance(x, y, px, py));

    float v2 = sin(sin(t)*x/(37+15*cos(y))) * cos(y/(31 +11*sin(x)));
    return (v1 + v2) /2;
    //return (sin(x/16.0) + sin(y/32.0) + sin(distance(x, t*y, 80, 12)/8.0) + sin(sqrt(x*x+y*y))/8.0) / 4.0;
}

int colors[] = { 0, 8, 7, 10 };

const char string [] = " oyarsa 0.1 lol martin testlol";


int kernel_main(multiboot_t *mboot)
{
    monitor_clear();
    monitor_write("Hello world!");
    monitor_put(' ');
    monitor_write_hex(0xdeadbeef);
    monitor_put(' ');
    monitor_write_dec(65535);
    monitor_put(' ');
    for (int i=0;i<=15;i++) monitor_put_styled('0'+i, i, 0);
    monitor_put('\n');

/*    monitor_clear();
    monitor_write("Hello world!");
    monitor_put(' ');
    monitor_write_hex(0xdeadbeef);
    monitor_put(' ');
    monitor_write_dec(65535);
    monitor_put(' ');
    for (int i=0;i<=15;i++) monitor_put_styled('0'+i, i, 0);
    monitor_put('\n');*/

    int t=0;
    int length = strlen(string);
    while(1) {
        monitor_move_cursor(0, 0);
        t++;
        for (int i=0; i<79; i++) {
            if (i < t % 80) monitor_put(' ');
            else if (i > t % 80 + length) break;
            else monitor_put(string[i - (t % 80)]);
        }
        monitor_put('\n');
        for (int y=0; y<22; y++) {
            for (int x=0; x<80; x++) {
                monitor_put_styled(' ', 0, colors[(int)(f(x, y, (float)t/20.0)*3)]);
            }
        }
        t %= 360 * length;
    }
   // panic("Testing!");

    return 0xdeadbeef;
}
