.section .text
.align 11
.global exception_vectors
.global exception_init
.global irq_enable
.global irq_disable

.macro vector_entry handler
    b \handler
    .align 7
.endm

exception_vectors:
    vector_entry unexpected_exception
    vector_entry unexpected_exception
    vector_entry unexpected_exception
    vector_entry unexpected_exception

    vector_entry unexpected_exception
    vector_entry current_el_irq
    vector_entry unexpected_exception
    vector_entry unexpected_exception

    vector_entry unexpected_exception
    vector_entry unexpected_exception
    vector_entry unexpected_exception
    vector_entry unexpected_exception

    vector_entry unexpected_exception
    vector_entry unexpected_exception
    vector_entry unexpected_exception
    vector_entry unexpected_exception

exception_init:
    ldr x0, =exception_vectors
    msr vbar_el1, x0
    isb
    ret

irq_enable:
    msr daifclr, #2
    ret

irq_disable:
    msr daifset, #2
    ret

current_el_irq:
    sub sp, sp, #(32 * 8)
    stp x0, x1, [sp, #(0 * 8)]
    stp x2, x3, [sp, #(2 * 8)]
    stp x4, x5, [sp, #(4 * 8)]
    stp x6, x7, [sp, #(6 * 8)]
    stp x8, x9, [sp, #(8 * 8)]
    stp x10, x11, [sp, #(10 * 8)]
    stp x12, x13, [sp, #(12 * 8)]
    stp x14, x15, [sp, #(14 * 8)]
    stp x16, x17, [sp, #(16 * 8)]
    stp x18, x19, [sp, #(18 * 8)]
    stp x20, x21, [sp, #(20 * 8)]
    stp x22, x23, [sp, #(22 * 8)]
    stp x24, x25, [sp, #(24 * 8)]
    stp x26, x27, [sp, #(26 * 8)]
    stp x28, x29, [sp, #(28 * 8)]
    str x30, [sp, #(30 * 8)]

    bl irq_handler

    ldp x0, x1, [sp, #(0 * 8)]
    ldp x2, x3, [sp, #(2 * 8)]
    ldp x4, x5, [sp, #(4 * 8)]
    ldp x6, x7, [sp, #(6 * 8)]
    ldp x8, x9, [sp, #(8 * 8)]
    ldp x10, x11, [sp, #(10 * 8)]
    ldp x12, x13, [sp, #(12 * 8)]
    ldp x14, x15, [sp, #(14 * 8)]
    ldp x16, x17, [sp, #(16 * 8)]
    ldp x18, x19, [sp, #(18 * 8)]
    ldp x20, x21, [sp, #(20 * 8)]
    ldp x22, x23, [sp, #(22 * 8)]
    ldp x24, x25, [sp, #(24 * 8)]
    ldp x26, x27, [sp, #(26 * 8)]
    ldp x28, x29, [sp, #(28 * 8)]
    ldr x30, [sp, #(30 * 8)]
    add sp, sp, #(32 * 8)
    eret

unexpected_exception:
    b unexpected_exception
