
#ifndef __STDIO_H
#define __STDIO_H

#include "lib/stdarg.h"
#include "type.h"


int printk(const char *fmt, ...);
u32 printk_port(const char *fmt, ...);


#endif /* __STDIO_H */
