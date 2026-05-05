# Bare-Metal Cortex-M3 Firmware on QEMU

This is a small bare-metal firmware project for an ARM Cortex-M3 system using QEMU's `mps2-an385` board. It boots from a hand-written vector table, initializes UART and SysTick, and runs a tiny command shell over the QEMU terminal.

The project stays close to a beginner-friendly embedded firmware layout: startup assembly, linker script, HAL drivers, and application code. No Arduino, RTOS, STM32Cube, PlatformIO, or external firmware framework is used.

## What It Does

- Boots from `startup/startup.s`
- Places code and data using `startup/linker.ld`
- Prints UART output through the QEMU terminal
- Uses SysTick for millisecond delays and tick counting
- Provides a small GPIO HAL for a `blink` shell command
- Includes a tiny UART-backed `printf`
- Runs a simple UART command shell
- Generates map, size, and disassembly files as project results

## Why This Matters

This project demonstrates the core pieces behind embedded firmware and SoC bring-up work:

- vector table and reset flow
- memory layout with FLASH and SRAM
- memory-mapped peripheral access
- polling UART driver
- timer interrupt handling
- simple HAL layering
- build/debug flow with an embedded cross toolchain

## Tools Used

- `arm-none-eabi-gcc`
- `arm-none-eabi-objdump`
- `arm-none-eabi-size`
- `qemu-system-arm`
- GNU Make

## Folder Structure

```text
bare-metal-soc/
├── startup/
│   ├── startup.s
│   └── linker.ld
├── src/
│   ├── main.c
│   ├── hal_uart.c
│   ├── hal_uart.h
│   ├── hal_timer.c
│   ├── hal_timer.h
│   ├── hal_gpio.c
│   ├── hal_gpio.h
│   ├── printf.c
│   └── printf.h
├── docs/
│   ├── memory_map.md
│   └── peripheral_regs.md
├── results/
│   ├── uart_log.txt
│   ├── size_report.txt
│   ├── disassembly.txt
│   └── firmware.map
├── Makefile
└── README.md
```

## Build

```sh
make clean
make
```

## Run

```sh
make run
```

This runs:

```sh
qemu-system-arm -M mps2-an385 -nographic -kernel firmware.elf
```

To quit QEMU in `-nographic` mode, press `Ctrl-a`, release, then press `x`.

## Debug

```sh
make debug
```

Then connect GDB from another terminal:

```sh
arm-none-eabi-gdb firmware.elf
target remote :1234
```

## Reports

```sh
make size
make disasm
make report
```

Generated files:

- `results/firmware.map`
- `results/size_report.txt`
- `results/disassembly.txt`
- `results/uart_log.txt`

## Expected UART Output

```text
Hello from Cortex-M3
UART is working
Tick
Tick
Tick
Command demo:
> help
Commands:
  help              show this list
  tick              print current SysTick ms count
  info              print board and memory info
  memread <addr>    read a 32-bit address, example 0xE000E010
  blink <n>         toggle GPIO n times
  reboot/reset      placeholder reset command
Interactive shell ready. Type 'help'.
>
```

## Shell Commands

| Command | Description |
| --- | --- |
| `help` | Print available commands |
| `tick` | Print current SysTick count in milliseconds |
| `info` | Print board, CPU, FLASH, SRAM, and clock info |
| `memread <hex address>` | Read a 32-bit memory address |
| `blink <n>` | Toggle GPIO output bit 0 `n` times |
| `reboot` / `reset` | Print a placeholder message |

## File Guide

| File | Purpose |
| --- | --- |
| `startup/startup.s` | Vector table, reset handler, `.data` copy, `.bss` zero |
| `startup/linker.ld` | FLASH/SRAM memory layout and section placement |
| `src/main.c` | Boot messages, demo commands, UART shell |
| `src/hal_uart.c` | CMSDK-style UART driver for QEMU terminal I/O |
| `src/hal_timer.c` | SysTick setup, interrupt handler, millisecond delay |
| `src/hal_gpio.c` | Small CMSDK-style GPIO driver |
| `src/printf.c` | Tiny UART-backed printf implementation |
| `docs/memory_map.md` | Project memory map |
| `docs/peripheral_regs.md` | Peripheral registers used by the HALs |

## Known Limitations

- UART input is intentionally simple: no command history or advanced line editing.
- `memread` can fault if pointed at an invalid or unsupported address.
- GPIO is modeled for the QEMU/CMSDK-style layout and is mainly used as a firmware exercise.
- Reset is left as a placeholder command.

## Future Improvements

- Add a safe software reset using the Cortex-M AIRCR register.
- Add UART interrupt-driven RX with a ring buffer.
- Add unit-style host tests for parsing helpers.
- Add a small GDB walkthrough in `docs/`.
