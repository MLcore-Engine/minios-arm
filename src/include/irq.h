#ifndef MINI_OS_IRQ_H
#define MINI_OS_IRQ_H

void exception_init(void);
void irq_init(void);
void irq_enable(void);
void irq_disable(void);
void irq_handler(void);

#endif
