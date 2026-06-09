#ifndef MINI_OS_TIMER_H
#define MINI_OS_TIMER_H

void timer_init(void);
void timer_handle_irq(void);
unsigned long timer_ticks(void);

#endif
