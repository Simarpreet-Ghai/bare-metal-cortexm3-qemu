#include <stdint.h>
#include "hal_gpio.h"

/* CMSDK-style GPIO block used by the QEMU mps2-an385 machine. */
#define GPIO0_BASE          0x40010000UL

#define GPIO_DATA           (*(volatile uint32_t *)(GPIO0_BASE + 0x000))
#define GPIO_DATAOUT        (*(volatile uint32_t *)(GPIO0_BASE + 0x004))
#define GPIO_OUTENSET       (*(volatile uint32_t *)(GPIO0_BASE + 0x010))

static uint32_t gpio_state = 0;

void gpio_init(void) {
  gpio_state = 0;
  GPIO_DATAOUT = gpio_state;
  GPIO_OUTENSET = 0xffffffffU;
}

void gpio_write(uint32_t value) {
  gpio_state = value;
  GPIO_DATAOUT = gpio_state;
}

uint32_t gpio_read(void) {
  return GPIO_DATA;
}

void gpio_toggle(uint32_t mask) {
  gpio_state ^= mask;
  GPIO_DATAOUT = gpio_state;
}
