#include <stdarg.h>
#include <stdint.h>
#include "hal_uart.h"
#include "printf.h"

static void print_unsigned(uint32_t value, uint32_t base) {
  char buffer[11];
  uint32_t index = 0;
  const char digits[] = "0123456789abcdef";

  if (value == 0) {
    uart_putchar('0');
    return;
  }

  while (value > 0 && index < sizeof(buffer)) {
    buffer[index++] = digits[value % base];
    value /= base;
  }

  while (index > 0) {
    uart_putchar(buffer[--index]);
  }
}

static void print_signed(int32_t value) {
  if (value < 0) {
    uart_putchar('-');
    print_unsigned((uint32_t)(-value), 10);
  } else {
    print_unsigned((uint32_t)value, 10);
  }
}

int printf(const char *fmt, ...) {
  va_list args;
  int count = 0;

  va_start(args, fmt);

  while (*fmt) {
    if (*fmt != '%') {
      uart_putchar(*fmt++);
      count++;
      continue;
    }

    fmt++;
    switch (*fmt) {
      case 's': {
        const char *s = va_arg(args, const char *);
        if (!s) {
          s = "(null)";
        }
        uart_puts(s);
        break;
      }
      case 'c':
        uart_putchar((char)va_arg(args, int));
        break;
      case 'd':
        print_signed(va_arg(args, int32_t));
        break;
      case 'u':
        print_unsigned(va_arg(args, uint32_t), 10);
        break;
      case 'x':
        print_unsigned(va_arg(args, uint32_t), 16);
        break;
      case '%':
        uart_putchar('%');
        break;
      default:
        uart_putchar('%');
        uart_putchar(*fmt);
        break;
    }

    if (*fmt) {
      fmt++;
    }
  }

  va_end(args);
  return count;
}
