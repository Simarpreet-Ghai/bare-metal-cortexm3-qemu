CC = arm-none-eabi-gcc

CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -g -Wall -ffreestanding -nostdlib
LDFLAGS = -T startup/linker.ld -nostdlib -Wl,-Map=results/firmware.map -lgcc

SRCS = startup/startup.s src/main.c src/hal_uart.c src/hal_timer.c
TARGET = firmware

all:
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET).elf
	arm-none-eabi-size $(TARGET).elf

clean:
	rm -f $(TARGET).elf results/firmware.map