.syntax unified
.cpu cortex-m3
.thumb

.section .vectors, "a", %progbits
.global vector_table
vector_table:
  .word 0x20005000
  .word reset_handler

.section .text, "ax", %progbits
.global reset_handler
reset_handler:
  mov  r0, #65
  movw r1, #32795
  ldr  r2, =539230465
1:
  b 1b

