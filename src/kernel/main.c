#include "uart.h"

void kernel_main(void)
{
    uart_init();
    uart_puts("miniOS ARM64 booting...\n");
    uart_puts("Hello from QEMU virt machine.\n");

    for (;;) {
        __asm__ volatile("wfe");
    }
}
