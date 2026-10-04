#ifndef CTXCONSOLE_H
#define CTXCONSOLE_H

#include <stddef.h>

int ctx_console_write(const char *text);
int ctx_console_write_n(const char *text, size_t length);

int ctx_console_printf(const char *format, ...);

int ctx_console_read(char *buffer, size_t size);

int ctx_console_execute(const char *command);

void ctx_console_clear(void);

#endif
