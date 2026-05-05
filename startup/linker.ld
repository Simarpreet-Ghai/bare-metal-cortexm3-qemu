/* startup/linker.ld */
ENTRY(Reset_Handler)

MEMORY {
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 512K
  SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}
SECTIONS {
  .text : {                         /* Code section in FLASH */
    KEEP(*(.vectors))               /* Interrupt vector table FIRST */
    *(.text*)                       /* All code */
    *(.rodata*)                     /* String literals, const data */
  } > FLASH
  _data_load = LOADADDR(.data);     /* Flash address of initialised data */
  .data : AT(_data_load) {          /* Initialised variables — live in SRAM, stored in FLASH */
    _data_start = .;
    *(.data*)
    _data_end = .;
  } > SRAM
  .bss : {                          /* Uninitialised variables — SRAM only */
    _bss_start = .;
    *(.bss*) *(COMMON)
    _bss_end = .;
  } > SRAM
  _stack_top = ORIGIN(SRAM) + LENGTH(SRAM);  /* Stack at top of SRAM */
}
