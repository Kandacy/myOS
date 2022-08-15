
#include "trap/trap.h"
#include "driver/riscv.h"
#include "lib/stdio.h"
#include "lib/error.h"
#include "syscall/syscall.h"
#include "task/task.h"

#if 0
impl Interrupt {
    pub fn from(nr: usize) -> Self {
        match nr {
            0 => Interrupt::UserSoft,
            1 => Interrupt::SupervisorSoft,
            2 => Interrupt::VirtualSupervisorSoft,
            4 => Interrupt::UserTimer,
            5 => Interrupt::SupervisorTimer,
            6 => Interrupt::VirtualSupervisorTimer,
            8 => Interrupt::UserExternal,
            9 => Interrupt::SupervisorExternal,
            10 => Interrupt::VirtualSupervisorExternal,
            _ => Interrupt::Unknown,
        }
    }
}
#endif




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
    u64 trap = scause & 0x0fff;
    if (scause & 0x8000 == 1) { // interrupt
        panic("can't identify interrupt trap");
    } else { // Exception
        switch (trap) {
            case UserEnvCall:
                cx->sepc += 4;
                cx->x_regs[10] = syscall(cx->x_regs[17], cx->x_regs[10], cx->x_regs[11], cx->x_regs[12]);
                break;
            case StoreFault:
                printk("[kernel] app err: store fault. run next\n");
                exit_current_app();
                break;
            default:
                panic("trap scause undefined.");
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
    printk("[Test] trap.__alltraps = 0x%x\n", (u64)__alltraps);
    // 重定向trap函数
    w_stvec( (u64)__alltraps | 0);
}
