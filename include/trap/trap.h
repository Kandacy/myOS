
#ifndef __TRAP_H
#define __TRAP_H

#include "type.h"


typedef struct TrapContext{
    u64 x_regs[32];
    u64 sstatus;
    u64 sepc;
}TrapContext;



void trap_init( void );


#endif /* __TRAP_H */
