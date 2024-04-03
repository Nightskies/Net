#ifndef TIMER_H
#define TIMER_H

#include "types.h"

#include <time.h>

struct timer
{
    clock_t start;
    clock_t end;
};

void timer_init(struct timer *timer);

void timer_start(struct timer *timer);

void timer_end(struct timer *timer);

f64 timer_get_elapsed_time(struct timer *timer);

void timer_delay(s32 milliseconds);

#endif // TIMER_H