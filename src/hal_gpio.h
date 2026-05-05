#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>

void gpio_init(void);
void gpio_write(uint32_t value);
uint32_t gpio_read(void);
void gpio_toggle(uint32_t mask);

#endif
