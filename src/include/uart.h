#ifndef MINI_OS_UART_H
#define MINI_OS_UART_H

void uart_init(void);
void uart_putc(char ch);
void uart_puts(const char *s);

#endif
