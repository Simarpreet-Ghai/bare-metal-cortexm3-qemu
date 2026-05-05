#include <stdint.h>
#include "hal_uart.h"

/* src/hal_uart.c */
#define UART0_BASE    0x40004000UL

#define UART_DATA     (*(volatile uint32_t *)(UART0_BASE + 0x000))
#define UART_STATE    (*(volatile uint32_t *)(UART0_BASE + 0x004))
#define UART_CTRL     (*(volatile uint32_t *)(UART0_BASE + 0x008))
#define UART_BAUDDIV  (*(volatile uint32_t *)(UART0_BASE + 0x010))

#define UART_STATE_TXFULL  (1 << 0)
#define UART_STATE_RXFULL  (1 << 1)
#define UART_CTRL_TXEN     (1 << 0)
#define UART_CTRL_RXEN     (1 << 1)

void uart_init(uint32_t baud) {
  uint32_t div = 24000000 / baud;

  UART_BAUDDIV = div;
  UART_CTRL = UART_CTRL_TXEN | UART_CTRL_RXEN;
}

void uart_putchar(char c) {
  while (UART_STATE & UART_STATE_TXFULL);

  UART_DATA = (uint32_t)c;
}

void uart_puts(const char *s) {
  while (*s) uart_putchar(*s++);
}

int uart_has_data(void) {
  return (UART_STATE & UART_STATE_RXFULL) != 0;
}

char uart_getchar(void) {
  while (!uart_has_data()) {
  }

  return (char)(UART_DATA & 0xffU);
}
