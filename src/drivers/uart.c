#include "uart.h"

#define UART0_BASE 0x09000000UL
#define UART_DR    0x00
#define UART_FR    0x18
#define UART_IBRD  0x24
#define UART_FBRD  0x28
#define UART_LCRH  0x2c
#define UART_CR    0x30
#define UART_IMSC  0x38
#define UART_ICR   0x44

#define UART_FR_TXFF (1U << 5)

static inline void mmio_write(unsigned long reg, unsigned int value)
{
    *(volatile unsigned int *)(UART0_BASE + reg) = value;
}

static inline unsigned int mmio_read(unsigned long reg)
{
    return *(volatile unsigned int *)(UART0_BASE + reg);
}

void uart_init(void)
{
    mmio_write(UART_CR, 0);
    mmio_write(UART_ICR, 0x7ff);
    mmio_write(UART_IBRD, 1);
    mmio_write(UART_FBRD, 40);
    mmio_write(UART_LCRH, (3U << 5));
    mmio_write(UART_IMSC, 0);
    mmio_write(UART_CR, (1U << 0) | (1U << 8) | (1U << 9));
}

void uart_putc(char ch)
{
    if (ch == '\n') {
        uart_putc('\r');
    }

    while (mmio_read(UART_FR) & UART_FR_TXFF) {
    }

    mmio_write(UART_DR, (unsigned int)ch);
}

void uart_puts(const char *s)
{
    while (*s) {
        uart_putc(*s++);
    }
}
