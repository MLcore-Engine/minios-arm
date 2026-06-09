#include "irq.h"
#include "log.h"
#include "timer.h"

void kernel_main(void)
{
    log_init();
    log_info("miniOS ARM64 booting...\n");
    log_info("initializing interrupts and timer\n");

    irq_init();
    timer_init();
    irq_enable();

    log_info("timer interrupt enabled\n");

    for (;;) {
        __asm__ volatile("wfe");
    }
}
