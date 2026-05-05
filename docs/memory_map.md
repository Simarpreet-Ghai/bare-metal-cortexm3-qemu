# Memory Map

This project targets the QEMU `mps2-an385` board, which models an ARM Cortex-M3 system.

| Region | Start | Size | Used for |
| --- | --- | --- | --- |
| FLASH | `0x00000000` | 512 KB | Vector table, code, read-only data |
| SRAM | `0x20000000` | 32 KB | `.data`, `.bss`, stack |

## Stack

The linker script places the stack top at the end of SRAM:

```text
0x20000000 + 32 KB = 0x20008000
```

The first word in the vector table is `_stack_top`, so the Cortex-M3 starts with `SP = 0x20008000`.

## Peripheral Bases

| Peripheral | Base address | Notes |
| --- | --- | --- |
| UART0 | `0x40004000` | CMSDK-style UART used for QEMU terminal I/O |
| GPIO0 | `0x40010000` | CCMSDK-style GPIO address used by this project; mainly used for the blink demo |
| SysTick | `0xE000E010` | Cortex-M system timer in the private peripheral space |

## Linker Sections

| Section | Location | Notes |
| --- | --- | --- |
| `.vectors` | FLASH at `0x00000000` | Initial stack pointer and exception vectors |
| `.text` | FLASH | Startup code and C functions |
| `.rodata` | FLASH | String constants |
| `.data` | SRAM, loaded from FLASH | Initialized global/static data |
| `.bss` | SRAM | Zero-initialized global/static data |
