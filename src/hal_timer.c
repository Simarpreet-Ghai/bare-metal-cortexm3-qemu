#include <stdint.h>
#include "hal_timer.h"

/* src/hal_timer.c */
#define SYSTICK_BASE  0xE000E010UL
#define SYST_CSR      (*(volatile uint32_t*)(SYSTICK_BASE + 0x0))
#define SYST_RVR      (*(volatile uint32_t*)(SYSTICK_BASE + 0x4))
#define SYST_CVR      (*(volatile uint32_t*)(SYSTICK_BASE + 0x8))

#define SYST_CSR_ENABLE     (1U << 0)
#define SYST_CSR_TICKINT    (1U << 1)
#define SYST_CSR_CLKSOURCE  (1U << 2)

static volatile uint32_t tick_count = 0;

void systick_init(uint32_t ticks_per_ms) {
  SYST_CSR = 0;
  SYST_RVR = ticks_per_ms - 1;
  SYST_CVR = 0;
  tick_count = 0;

  __asm volatile ("cpsie i");

  SYST_CSR = SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE;
}

void SysTick_Handler(void) {
  tick_count++;
}

uint32_t get_tick_ms(void) {
  return tick_count;
}

void delay_ms(uint32_t ms) {
  uint32_t start = get_tick_ms();

  while ((get_tick_ms() - start) < ms) {
  }
}
