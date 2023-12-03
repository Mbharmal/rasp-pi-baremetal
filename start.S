.section ".text.boot"

.global _start

_start:
    // Check for the primary core
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbnz    x1, halt_here

    // TODO: Find a better place for the stack, for now it is just above the .text section
    ldr     x1, =_start
    mov     sp, x1

    // Zero out .bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
1:  cbz     w2, jump_to_main
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 1b

    // Jump to C envrionment
jump_to_main:
    bl      main
    b       halt_here

    // Non-primary core, loop forever
halt_here:
    wfe
    b       halt_here
