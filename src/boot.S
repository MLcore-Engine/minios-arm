.section .text.boot
.global _start

_start:
    ldr x0, =__bss_start
    ldr x1, =__bss_end

clear_bss:
    cmp x0, x1
    b.hs bss_done
    str xzr, [x0], #8
    b clear_bss

bss_done:
    ldr x0, =__stack_top
    mov sp, x0
    bl kernel_main

hang:
    wfe
    b hang
