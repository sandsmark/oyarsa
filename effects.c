#include "effects.h"
#include "math.h"
#include "monitor.h"

static float f(float x, float y, float t)
{
//    return sin(x/40.0 +t);
    //return sin(distance(x, y, (40 * sin(-t) + 40), (12 * cos(-t) + 12)) / 40.74);
    return (sin(x/16.0) + sin(y/32.0) + sin(distance(x, t*y, 80, 12)/8.0) + sin(sqrt(x*x+y*y))/8.0) / 4.0;
}

void effects_run()
{
    monitor_put(' ');
    monitor_write_hex(0xdeadbeef);
    monitor_put(' ');
    monitor_write_dec(65535);
    monitor_put(' ');
    for (int i=0;i<=15;i++) monitor_put_styled('0'+i, i, 0);
    monitor_put('\n');

/*    float t=0;
    while(1) {
        monitor_move_cursor(0, 1);
        t+=0.1;
        for (int y=0; y<23; y++) {
            for (int x=0; x<80; x++) {
                monitor_put_styled(' ', 0, (int)(f(x, y, t)*15));
            }
        }
    }*/
}
