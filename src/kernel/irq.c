#include "irq.h"
#include "log.h"
#include "timer.h"

#define GICD_BASE 0x08000000UL
#define GICC_BASE 0x08010000UL

#define GICD_CTLR       0x000
#define GICD_ISENABLER  0x100
#define GICD_IPRIORITYR 0x400

#define GICC_CTLR 0x00
#define GICC_PMR  0x04
#define GICC_IAR  0x0c
#define GICC_EOIR 0x10

#define IRQ_CNTPNSIRQ 30U
#define IRQ_SPURIOUS  1023U

static inline void mmio_write32(unsigned long addr, unsigned int value)
{
    *(volatile unsigned int *)addr = value;
}

static inline unsigned int mmio_read32(unsigned long addr)
{
    return *(volatile unsigned int *)addr;
}

static inline void mmio_write8(unsigned long addr, unsigned char value)
{
    *(volatile unsigned char *)addr = value;
}

static void gic_enable_irq(unsigned int irq)
{
    mmio_write8(GICD_BASE + GICD_IPRIORITYR + irq, 0x80);
    mmio_write32(GICD_BASE + GICD_ISENABLER + ((irq / 32U) * 4U), 1U << (irq % 32U));
}

static void gic_init(void)
{
    mmio_write32(GICD_BASE + GICD_CTLR, 0);
    mmio_write32(GICC_BASE + GICC_CTLR, 0);

    gic_enable_irq(IRQ_CNTPNSIRQ);

    mmio_write32(GICC_BASE + GICC_PMR, 0xff);
    mmio_write32(GICC_BASE + GICC_CTLR, 1);
    mmio_write32(GICD_BASE + GICD_CTLR, 1);
}

static unsigned int gic_acknowledge(void)
{
    return mmio_read32(GICC_BASE + GICC_IAR) & 0x3ffU;
}

static void gic_end(unsigned int irq)
{
    mmio_write32(GICC_BASE + GICC_EOIR, irq);
}

void irq_init(void)
{
    exception_init();
    gic_init();
}

void irq_handler(void)
{
    unsigned int irq = gic_acknowledge();

    if (irq == IRQ_SPURIOUS) {
        return;
    }

    if (irq == IRQ_CNTPNSIRQ) {
        timer_handle_irq();
    } else {
        log_warn("unexpected irq %u\n", irq);
    }

    gic_end(irq);
}
