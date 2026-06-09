#ifndef MINI_OS_LOG_H
#define MINI_OS_LOG_H

void log_init(void);
void log_printf(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_warn(const char *fmt, ...);
void log_panic(const char *fmt, ...);

#endif
