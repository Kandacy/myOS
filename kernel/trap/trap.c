
#include "trap/trap.h"
#include "driver/riscv.h"
#include "lib/stdio.h"
#include "lib/error.h"
#include "syscall/syscall.h"

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

impl Exception {
    pub fn from(nr: usize) -> Self {
        match nr {
            0 => Exception::InstructionMisaligned,
            1 => Exception::InstructionFault,
            2 => Exception::IllegalInstruction,
            3 => Exception::Breakpoint,
            5 => Exception::LoadFault,
            6 => Exception::StoreMisaligned,
            7 => Exception::StoreFault,
            8 => Exception::UserEnvCall,
            10 => Exception::VirtualSupervisorEnvCall,
            12 => Exception::InstructionPageFault,
            13 => Exception::LoadPageFault,
            15 => Exception::StorePageFault,
            20 => Exception::InstructionGuestPageFault,
            21 => Exception::LoadGuestPageFault,
            22 => Exception::VirtualInstruction,
            23 => Exception::StoreGuestPageFault,
            _ => Exception::Unknown,
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
