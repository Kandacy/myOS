
#include "trap/trap.h"
#include "driver/riscv.h"
#include "lib/stdio.h"
#include "lib/error.h"
#include "mm/mem.h"
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




void trap_from_kernel( void ){
    panic("[trap/trap.c] kernel trap, panic");
}



void kernel_trap_entry_set( void ){
    w_stvec( (u64)trap_from_kernel | 0); // 开启地址空间后__alltraps的虚拟地址
}




void user_trap_entry_set( void ){
    w_stvec( (u64)TRAMPOLINE_FULL | 0); // 开启地址空间后__alltraps的虚拟地址
}



void trap_return( void ){

    // 恢复trap地址为__alltraps
    user_trap_entry_set();

    VirtAddr restore_va = (VirtAddr)__restore - (VirtAddr)__alltraps + TRAMPOLINE_FULL;
    u64 current_app_id = get_current_app_id();
    u64 current_app_user_satp = get_current_user_satp();

    asm volatile("fence.i");
    register u64 a0 asm("a0") = (u64)(TRAMPOLINE_FULL - sizeof(TrapContext)); 
    register u64 a1 asm("a1") = (u64)(current_app_user_satp);
    register u64 a2 asm("a2") = (u64)(restore_va);  
    asm volatile("jr a2");
}




/**
 *  @brief: trap中断服务函数
 *  @param:
 *  @return: 返回去的reg
 */
void trap_handler( void ) {

    TrapContext *cx = get_current_trap_cx();
    // printk("[trap_handler] cx = 0x%lx\n", (u64)cx);
    
    // kernel_trap_entry_set();

    // printk("[kernel] trap handler\n");
    u64 scause = r_scause();
    u64 stval = r_stval();

    
    // 根据原因处理trap
    if (scause & 0x8000 != 0) { // interrupt
        u64 trap = scause & 0x0fff;
        switch (trap) {
            case SupervisorTimer:
                // printk("[trap_handler] timer trap\n");
                suspend_current_app();
                set_next_trigger();
                break;
            default:
                printk("[Error] trap: interrupt %d undefined.\n", trap);
                panic("trap interrupt case undefined");
                break;
        }
    } else { // Exception
        switch (scause) {
            case UserEnvCall:
                cx->sepc += 4;
                // for(int i = 0; i < 20; i ++){
                //     printk("[%ld] = %lx\n", i, cx->x_regs[i]);
                // }
                cx->x_regs[10] = syscall(cx->x_regs[17], cx->x_regs[10], cx->x_regs[11], cx->x_regs[12]);
                break;
            case StoreFault:
                printk("[kernel] app err: store fault. run next\n");
                exit_current_app();
                break;
            default:
                printk("[Error] trap: exception %d undefined.\n", scause);
                panic("trap exception case undefined.");
                break;
        }
    }

    trap_return();
}




/**
 *  @brief: trap初始化，重定向trap入口
 *  @param:
 *  @return:
 */
void trap_init( void ){
    // printk("[Test] trap.__alltraps = 0x%x\n", (u64)__alltraps);
    // 重定向trap函数
    // w_stvec( (u64)__alltraps | 0);
    user_trap_entry_set();
}
