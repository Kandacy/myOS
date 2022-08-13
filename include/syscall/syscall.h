
#ifndef __SYSCALL_H
#define __SYSCALL_H

#include "type.h"


/* fs.c */
extern u64 sys_write(u64 fd, char *buf, u64 len);
extern void sys_exit(u64 exit_id);

/* syscall.c */
u64 syscall(u64 id, u64 arg0, u64 arg1, u64 arg2);

#endif /* __SYSCALL_H */
