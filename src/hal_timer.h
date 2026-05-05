#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include <stdint.h>

void systick_init(uint32_t ticks_per_ms);
uint32_t get_tick_ms(void);
void delay_ms(uint32_t ms);

#endif
