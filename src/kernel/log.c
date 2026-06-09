#include "log.h"
#include "uart.h"

typedef __builtin_va_list va_list;

#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type)   __builtin_va_arg(ap, type)
#define va_end(ap)         __builtin_va_end(ap)

static void print_unsigned(unsigned long value, unsigned int base)
{
    char buf[32];
    unsigned int i = 0;

    if (value == 0) {
        uart_putc('0');
        return;
    }

    while (value > 0) {
        unsigned int digit = value % base;
        buf[i++] = (digit < 10) ? (char)('0' + digit) : (char)('a' + digit - 10);
        value /= base;
    }

    while (i > 0) {
        uart_putc(buf[--i]);
    }
}

static void print_signed(long value)
{
    if (value < 0) {
        uart_putc('-');
        print_unsigned((unsigned long)-value, 10);
        return;
    }

    print_unsigned((unsigned long)value, 10);
}

static void vlog_printf(const char *fmt, va_list args)
{
    while (*fmt) {
        if (*fmt != '%') {
            uart_putc(*fmt++);
            continue;
        }

        fmt++;

        switch (*fmt) {
        case '%':
            uart_putc('%');
            break;
        case 'c':
            uart_putc((char)va_arg(args, int));
            break;
        case 's': {
            const char *s = va_arg(args, const char *);
            uart_puts(s ? s : "(null)");
            break;
        }
        case 'd':
            print_signed((long)va_arg(args, int));
            break;
        case 'u':
            print_unsigned((unsigned long)va_arg(args, unsigned int), 10);
            break;
        case 'x':
            print_unsigned((unsigned long)va_arg(args, unsigned int), 16);
            break;
        case 'p':
            uart_puts("0x");
            print_unsigned((unsigned long)va_arg(args, void *), 16);
            break;
        default:
            uart_putc('%');
            uart_putc(*fmt);
            break;
        }

        fmt++;
    }
}

void log_init(void)
{
    uart_init();
}

void log_printf(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vlog_printf(fmt, args);
    va_end(args);
}

void log_info(const char *fmt, ...)
{
    va_list args;

    uart_puts("[info] ");
    va_start(args, fmt);
    vlog_printf(fmt, args);
    va_end(args);
}

void log_warn(const char *fmt, ...)
{
    va_list args;

    uart_puts("[warn] ");
    va_start(args, fmt);
    vlog_printf(fmt, args);
    va_end(args);
}

void log_panic(const char *fmt, ...)
{
    va_list args;

    uart_puts("[panic] ");
    va_start(args, fmt);
    vlog_printf(fmt, args);
    va_end(args);

    for (;;) {
        __asm__ volatile("wfe");
    }
}
