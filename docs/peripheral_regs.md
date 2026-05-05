# Peripheral Registers

## UART0

The UART driver uses the QEMU `mps2-an385` CMSDK-style UART layout. It does not use PL011 offsets.

Base: `0x40004000`

| Register | Offset | Use |
| --- | --- | --- |
| `UART_DATA` | `0x000` | Write transmit byte, read received byte |
| `UART_STATE` | `0x004` | TX/RX status bits |
| `UART_CTRL` | `0x008` | Enable TX and RX |
| `UART_BAUDDIV` | `0x010` | Baud divisor |

Bits used:

| Bit | Meaning |
| --- | --- |
| `UART_STATE[0]` | TX full |
| `UART_STATE[1]` | RX full |
| `UART_CTRL[0]` | TX enable |
| `UART_CTRL[1]` | RX enable |

## SysTick

Base: `0xE000E010`

| Register | Offset | Use |
| --- | --- | --- |
| `SYST_CSR` | `0x000` | Control and status |
| `SYST_RVR` | `0x004` | Reload value |
| `SYST_CVR` | `0x008` | Current value, cleared by writing any value |

Bits used in `SYST_CSR`:

| Bit | Meaning |
| --- | --- |
| `0` | Enable counter |
| `1` | Enable SysTick interrupt |
| `2` | Use processor clock |

With a 24 MHz clock, the firmware loads `24000 - 1` to generate a 1 ms interrupt tick.

## GPIO0

Base assumption: `0x40010000`

The GPIO HAL is intentionally small. It is used by the shell's `blink <n>` command as a QEMU-safe firmware exercise.

| Register | Offset | Use |
| --- | --- | --- |
| `GPIO_DATA` | `0x000` | Read GPIO input state |
| `GPIO_DATAOUT` | `0x004` | Write GPIO output state |
| `GPIO_OUTENSET` | `0x010` | Enable output bits |

This follows the CMSDK-style GPIO layout commonly paired with the CMSDK UART on `mps2-an385`.
