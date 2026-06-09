#include "log.h"
#include "timer.h"

static unsigned long tick_count;
static unsigned int ticks_per_second;

static inline unsigned int read_cntfrq(void)
{
    unsigned long value;

    __asm__ volatile("mrs %0, cntfrq_el0" : "=r"(value));
    return (unsigned int)value;
}

static inline void write_cntp_tval(unsigned int value)
{
    __asm__ volatile("msr cntp_tval_el0, %0" : : "r"((unsigned long)value));
}

static inline void write_cntp_ctl(unsigned int value)
{
    __asm__ volatile("msr cntp_ctl_el0, %0" : : "r"((unsigned long)value));
}

static void timer_program_next_tick(void)
{
    write_cntp_tval(ticks_per_second);
    write_cntp_ctl(1);
}

void timer_init(void)
{
    ticks_per_second = read_cntfrq();
    tick_count = 0;

    log_info("timer frequency %u Hz\n", ticks_per_second);
    timer_program_next_tick();
}

void timer_handle_irq(void)
{
    tick_count++;
    log_info("tick %u\n", (unsigned int)tick_count);
    timer_program_next_tick();
}

unsigned long timer_ticks(void)
{
    return tick_count;
}
