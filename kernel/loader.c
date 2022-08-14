
/**
 *  加载app到指定的内存地址
 */


#include "lib/stdio.h"
#include "trap/trap.h"
#include "driver/riscv.h"
#include "lib/error.h"


#define APP_BASE_ADDRESS  0x80400000
#define KERNEL_STACK_SIZE 4096
#define USER_STACK_SIZE   4096
#define APP_MAX_SIZE      0x20000


extern void _num_app(void);


static u64 current_app = 0;

static u8 kernel_stack[KERNEL_STACK_SIZE] = {0};
static u8 user_stack[USER_STACK_SIZE] = {0};



u64 get_kernel_stack_top( void ) {
    return (u64)(kernel_stack) + KERNEL_STACK_SIZE;
}


u64 get_user_stack_top( void ) {
    return (u64)(user_stack) + KERNEL_STACK_SIZE;
}



/**
 *  @brief: 加载app到0x8040_0000
 *  @param:
 *  @return:
 */
void load_app( void ) {
    
    u64 *num_app_ptr = (u64 *)_num_app;
    // app数量
    u64 num_app = *num_app_ptr;
    num_app_ptr ++; // 现在num_app_ptr是app文件起始位置

    // 刷新缓冲区
    asm volatile("fence.i");

    for(u64 i = 0; i < num_app; i ++){
        u8 *app_addr;
        app_addr = (u8 *)(APP_BASE_ADDRESS + APP_MAX_SIZE * i);
        printk("[Test] loader: app 0x%x - 0x%x\n", num_app_ptr[i], num_app_ptr[i + 1]);
        for(u8 *target = (u8 *)num_app_ptr[i]; (u64)target <= (u64)num_app_ptr[i + 1]; target ++, app_addr ++){
            *app_addr = *target;
        }
    }
}




/**
 *  @brief: 调用__restore，启动程序
 *  @param:
 *  @return: 
 */
void run_app( void ) {

    if(current_app >= *(u64 *)_num_app){
        panic("all app execed");
    }

    /* 设置tc，准备传给app */
    const u64 sstatus = r_sstatus();
    static TrapContext tc = {
        {0},
        0,
        0
    };
    tc.sepc = APP_BASE_ADDRESS + (APP_MAX_SIZE * current_app);
    current_app ++;
    tc.sstatus = sstatus & (~SSTATUS_SPP);
    tc.x_regs[2] = get_user_stack_top(); // 保证sscratch指向user_stack

    // 将tc压入kernel_stack
    u64 store_posi = get_kernel_stack_top() - sizeof(TrapContext);
    *(TrapContext *)store_posi = tc;
    
    extern void __restore(TrapContext *cx);
    __restore(&tc);
}
