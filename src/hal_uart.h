#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>

void uart_init(uint32_t baud);
void uart_putchar(char c);
void uart_puts(const char *s);
int uart_has_data(void);
char uart_getchar(void);

#endif
