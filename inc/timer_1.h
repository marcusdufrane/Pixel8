//Timer setup
#ifndef _TIMER_1_H
#define _TIMER_1_H

#include "stdio.h"

static volatile uint8_t timer_100ms_counter = 0;

void configure_10ms_timer();

#endif //_TIMER_1_H