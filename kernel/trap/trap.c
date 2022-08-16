
#include "trap/trap.h"
#include "driver/riscv.h"
#include "lib/stdio.h"
#include "lib/error.h"
#include "syscall/syscall.h"
#include "task/task.h"
#include "timer.h"



/* interrupt */
#define UserSoft                   0  
#define SupervisorSoft             1  
#define VirtualSupervisorSoft      2  
#define UserTimer                  4  
#define SupervisorTimer            5  
#define VirtualSupervisorTimer     6  
#define UserExternal               8  
#define SupervisorExternal         9  
#define VirtualSupervisorExternal  10 

/* exception */
#define InstructionMisaligned       0
#define InstructionFault            1
#define IllegalInstruction          2
#define Breakpoint                  3
#define LoadFault                   5
#define StoreMisaligned             6
#define StoreFault                  7
#define UserEnvCall                 8
#define VirtualSupervisorEnvCall    10
#define InstructionPageFault        12
#define LoadPageFault               13
#define StorePageFault              15
#define InstructionGuestPageFault   20
#define LoadGuestPageFault          21
#define VirtualInstruction          22
#define StoreGuestPageFault         23



extern void __alltraps(void);
extern void __restore(TrapContext *cx);




/**
 *  @brief: trap中断服务函数
 *  @param:
 *      cx: trap.s传入的要保存的reg
 *  @return: 返回去的reg
 */
TrapContext *trap_handler(TrapContext *cx) {
    // printk("[kernel] trap handler\n");
    u64 scause = r_scause();
    u64 stval = r_stval();

    // printk("[Trap] scause = 0x%x\n", scause);
    
    // 根据原因处理trap
    if (scause & 0x8000 != 0) { // interrupt
        u64 trap = scause & 0x0fff;
        switch (trap) {
            case SupervisorTimer:
                set_next_trigger();
                suspend_current_app();
                break;
            default:
                panic("trap interrupt case undefined");
                break;
        }
    } else { // Exception
        switch (scause) {
            case UserEnvCall:
                cx->sepc += 4;
                cx->x_regs[10] = syscall(cx->x_regs[17], cx->x_regs[10], cx->x_regs[11], cx->x_regs[12]);
                break;
            case StoreFault:
                printk("[kernel] app err: store fault. run next\n");
                exit_current_app();
                break;
            default:
                panic("trap exception case undefined.");
                break;
        }
    }

    return cx;
}




/**
 *  @brief: trap初始化，重定向trap入口
 *  @param:
 *  @return:
 */
void trap_init( void ){
    // printk("[Test] trap.__alltraps = 0x%x\n", (u64)__alltraps);
    // 重定向trap函数
    w_stvec( (u64)__alltraps | 0);
}
