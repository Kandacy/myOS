
#ifndef __TRAP_H
#define __TRAP_H

#include "type.h"


typedef struct TrapContext{
    u64 x_regs[32];
    u64 sstatus;
    u64 sepc;
    u64 kernel_satp;
    u64 kernel_sp; // 用户内核栈的物理地址（内核虚拟地址）
    u64 trap_handler;
}TrapContext;



void trap_init( void );


#endif /* __TRAP_H */
