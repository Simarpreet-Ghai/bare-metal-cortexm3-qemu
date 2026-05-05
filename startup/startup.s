.syntax unified
.cpu cortex-m3
.thumb

.global Reset_Handler
.global Default_Handler
.extern main
.extern SysTick_Handler

.section .vectors, "a", %progbits
.word _stack_top
.word Reset_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word 0
.word 0
.word 0
.word 0
.word Default_Handler
.word Default_Handler
.word 0
.word Default_Handler
.word SysTick_Handler
Reset_Handler:
    ldr r0, =_data_load
    ldr r1, =_data_start
    ldr r2, =_data_end

copy_data:
    cmp r1, r2
    bge zero_bss
    ldr r3, [r0]
    str r3, [r1]
    adds r0, r0, #4
    adds r1, r1, #4
    b copy_data

zero_bss:
    ldr r1, =_bss_start
    ldr r2, =_bss_end
    movs r3, #0

zero_loop:
    cmp r1, r2
    bge call_main
    str r3, [r1]
    adds r1, r1, #4
    b zero_loop

call_main:
    bl main

hang:
    b hang

.section .text.Default_Handler, "ax", %progbits
.thumb_func
Default_Handler:
    b Default_Handler
    