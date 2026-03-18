.syntax unified
.cpu cortex-m3
.thumb

.word 0x20000400
.word 0x080000ed
.space 0xe4

mov r0, #65
movw r1, #32795
ldr r2, =539230464
B .


//Random_code:dzstu9dp7kp