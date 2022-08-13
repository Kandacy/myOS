
#ifndef __SYSCALL_H
#define __SYSCALL_H


#include "type.h"


i64 sys_write(u64 fd, char *buf);
void sys_exit(i32 id);


#endif /* __SYSCALL_H */
