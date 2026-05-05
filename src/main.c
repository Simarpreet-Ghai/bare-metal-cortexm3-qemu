#include <stdint.h>
#include "hal_gpio.h"
#include "hal_timer.h"
#include "hal_uart.h"
#include "printf.h"

#define CPU_CLOCK_HZ  24000000U

static int str_equal(const char *a, const char *b) {
  while (*a && *b && (*a == *b)) {
    a++;
    b++;
  }

  return *a == *b;
}

static int str_starts_with(const char *s, const char *prefix) {
  while (*prefix) {
    if (*s++ != *prefix++) {
      return 0;
    }
  }

  return 1;
}

static const char *skip_spaces(const char *s) {
  while (*s == ' ') {
    s++;
  }

  return s;
}

static int parse_uint(const char *s, uint32_t *value) {
  uint32_t result = 0;
  int found = 0;

  s = skip_spaces(s);
  while (*s >= '0' && *s <= '9') {
    result = (result * 10U) + (uint32_t)(*s - '0');
    s++;
    found = 1;
  }

  if (!found || *skip_spaces(s) != '\0') {
    return 0;
  }

  *value = result;
  return 1;
}

static int hex_digit(char c, uint32_t *value) {
  if (c >= '0' && c <= '9') {
    *value = (uint32_t)(c - '0');
    return 1;
  }
  if (c >= 'a' && c <= 'f') {
    *value = (uint32_t)(c - 'a' + 10);
    return 1;
  }
  if (c >= 'A' && c <= 'F') {
    *value = (uint32_t)(c - 'A' + 10);
    return 1;
  }

  return 0;
}

static int parse_hex(const char *s, uint32_t *value) {
  uint32_t result = 0;
  uint32_t digit = 0;
  int found = 0;

  s = skip_spaces(s);
  if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
    s += 2;
  }

  while (hex_digit(*s, &digit)) {
    result = (result << 4) | digit;
    s++;
    found = 1;
  }

  if (!found || *skip_spaces(s) != '\0') {
    return 0;
  }

  *value = result;
  return 1;
}

static void print_help(void) {
  printf("Commands:\r\n");
  printf("  help              show this list\r\n");
  printf("  tick              print current SysTick ms count\r\n");
  printf("  info              print board and memory info\r\n");
  printf("  memread <addr>    read a 32-bit address, example 0xE000E010\r\n");
  printf("  blink <n>         toggle GPIO n times\r\n");
  printf("  reboot/reset      placeholder reset command\r\n");
}

static void print_info(void) {
  printf("Board: QEMU mps2-an385\r\n");
  printf("CPU: ARM Cortex-M3\r\n");
  printf("FLASH: 0x00000000, 512 KB\r\n");
  printf("SRAM:  0x20000000, 32 KB\r\n");
  printf("Clock used for UART/SysTick: %u Hz\r\n", CPU_CLOCK_HZ);
}

static void command_execute(const char *line) {
  uint32_t value = 0;

  line = skip_spaces(line);

  if (*line == '\0') {
    return;
  }

  if (str_equal(line, "help")) {
    print_help();
  } else if (str_equal(line, "tick")) {
    printf("tick=%u ms\r\n", get_tick_ms());
  } else if (str_equal(line, "info")) {
    print_info();
  } else if (str_starts_with(line, "memread ")) {
    if (parse_hex(line + 8, &value)) {
      uint32_t read_value = *(volatile uint32_t *)value;
      printf("0x%x: 0x%x\r\n", value, read_value);
    } else {
      printf("Usage: memread <hex address>\r\n");
    }
  } else if (str_starts_with(line, "blink ")) {
    if (parse_uint(line + 6, &value)) {
      printf("Blinking GPIO %u times\r\n", value);
      for (uint32_t i = 0; i < value; i++) {
        gpio_toggle(1U);
        delay_ms(100);
        gpio_toggle(1U);
        delay_ms(100);
      }
      printf("Done\r\n");
    } else {
      printf("Usage: blink <count>\r\n");
    }
  } else if (str_equal(line, "reboot") || str_equal(line, "reset")) {
    printf("Reset is not implemented in this guide project.\r\n");
  } else {
    printf("Unknown command: %s\r\n", line);
    printf("Type 'help' for commands.\r\n");
  }
}

static void read_line(char *buffer, uint32_t size) {
  uint32_t index = 0;

  while (1) {
    char c = uart_getchar();

    if (c == '\r' || c == '\n') {
      uart_puts("\r\n");
      buffer[index] = '\0';
      return;
    }

    if ((c == '\b' || c == 127) && index > 0) {
      index--;
      uart_puts("\b \b");
      continue;
    }

    if (c >= 32 && c <= 126 && index < (size - 1U)) {
      buffer[index++] = c;
      uart_putchar(c);
    }
  }
}

static void run_demo_commands(void) {
  const char *demo[] = {
    "help",
    "info",
    "tick",
    "memread 0xE000E010",
    "blink 2",
    "reset"
  };

  printf("Command demo:\r\n");
  for (uint32_t i = 0; i < (sizeof(demo) / sizeof(demo[0])); i++) {
    printf("> %s\r\n", demo[i]);
    command_execute(demo[i]);
  }
}

int main(void) {
  char line[80];

  uart_init(115200);
  systick_init(CPU_CLOCK_HZ / 1000U);
  gpio_init();

  printf("Hello from Cortex-M3\r\n");
  printf("UART is working\r\n");

  for (uint32_t i = 0; i < 3; i++) {
    printf("Tick\r\n");
    delay_ms(1000);
  }

  run_demo_commands();
  printf("Interactive shell ready. Type 'help'.\r\n");
  while (1) {
    printf("> ");
    read_line(line, sizeof(line));
    command_execute(line);
  }
}
