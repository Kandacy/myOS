
/**
 *  加载app到指定的内存地址
 */


#include "lib/stdio.h"
#include "trap/trap.h"
#include "driver/riscv.h"
#include "lib/error.h"
#include "mm/mem.h"
#include "config.h"



extern void _num_app(void);



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



