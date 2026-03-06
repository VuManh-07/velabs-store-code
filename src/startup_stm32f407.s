/* Minimal startup file for STM32F407 — Renode simulation */
    .syntax unified
    .cpu cortex-m4
    .thumb

    .section .isr_vector,"a",%progbits
    .type g_pfnVectors, %object
g_pfnVectors:
    .word  _estack
    .word  Reset_Handler
    .word  NMI_Handler
    .word  HardFault_Handler
    .word  0, 0, 0, 0, 0, 0, 0
    .word  SVC_Handler
    .word  0, 0
    .word  PendSV_Handler
    .word  SysTick_Handler

    .section .text.Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
    ldr   sp, =_estack

    ldr   r0, =_sdata
    ldr   r1, =_edata
    ldr   r2, =_sidata
    movs  r3, #0
    b     LoopCopyDataInit
CopyDataInit:
    ldr   r4, [r2, r3]
    str   r4, [r0, r3]
    adds  r3, r3, #4
LoopCopyDataInit:
    adds  r4, r0, r3
    cmp   r4, r1
    bcc   CopyDataInit

    ldr   r2, =_sbss
    ldr   r4, =_ebss
    movs  r3, #0
    b     LoopFillZerobss
FillZerobss:
    str   r3, [r2]
    adds  r2, r2, #4
LoopFillZerobss:
    cmp   r2, r4
    bcc   FillZerobss

    bl    main
LoopForever:
    b     LoopForever

    .weak NMI_Handler
    .thumb_set NMI_Handler, .
    .weak HardFault_Handler
    .thumb_set HardFault_Handler, .
    .weak SVC_Handler
    .thumb_set SVC_Handler, .
    .weak PendSV_Handler
    .thumb_set PendSV_Handler, .
    .weak SysTick_Handler
    .thumb_set SysTick_Handler, .
