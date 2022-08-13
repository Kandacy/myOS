
/**
 *  加载app到指定的内存地址
 */


#include "lib/stdio.h"
#include "trap/trap.h"
#include "driver/riscv.h"


#define APP_BASE_ADDRESS  0x80400000
#define KERNEL_STACK_SIZE 4096


extern void app_0_start(void);
extern void app_0_end(void);


static u8 kernel_stack[KERNEL_STACK_SIZE] = {0};



u64 get_kernel_stack_top( void ) {
    return (u64)(kernel_stack) + KERNEL_STACK_SIZE;
}



/**
 *  @brief: 加载app到0x8040_0000
 *  @param:
 *  @return:
 */
void load_app( void ) {

    // 刷新缓冲区
    asm volatile("fence.i");

    u8 *app_addr;
    app_addr = (u8 *)APP_BASE_ADDRESS;
    printk("[Test] loader: app 0x%x - 0x%x\n", app_0_start, app_0_end);
    printk("[Test] nei = %x\n", *app_addr);
    for(u8 *target = (u8 *)app_0_start; (u64)target <= (u64)app_0_end; target ++, app_addr ++){
        *app_addr = *target;
    }
    printk("[Test] nei = %x\n", *(u8 *)APP_BASE_ADDRESS);
}




/**
 *  @brief: 调用__restore，启动程序
 *  @param:
 *  @return: 
 */
void run_app( void ) {

    /* 设置tc，准备传给app */
    const u64 sstatus = r_sstatus();
    static TrapContext tc = {
        {0},
        0,
        APP_BASE_ADDRESS
    };
    tc.sstatus = sstatus & (~SSTATUS_SPP);
    tc.x_regs[2] = get_kernel_stack_top();

    // 将tc压入kernel_stack
    u64 store_posi = get_kernel_stack_top() - sizeof(TrapContext);
    *(TrapContext *)store_posi = tc;
    
    extern void __restore(TrapContext *cx);
    __restore(&tc);
}
