#include "timer.h"

#include <strings.h>

void timer_init(struct timer *timer)
{
    bzero(timer, sizeof(struct timer));
}

void timer_start(struct timer *timer)
{
    timer->start = clock();
}

void timer_end(struct timer *timer)
{
    timer->end = clock();
}

f64 timer_get_elapsed_time(struct timer *timer)
{
    return (f64)(timer->end - timer->start) * 1000.0 / CLOCKS_PER_SEC;
}

void timer_delay(s32 milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}