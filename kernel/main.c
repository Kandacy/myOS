
/**
 *  @author: Song XiuMing
 *  @date: 22/08/11
 */


// #include "start.s"
#include "type.h"
#include "lib/stdio.h"
#include "lib/error.h"
#include "lib/string.h"
#include "trap/trap.h"
#include "loader.h"
#include "task/task.h"
#include "timer.h"


// extern void boot_stack(void);
// extern void boot_stack_top(void);
// extern u8 *boot_stack;
// extern u8 *boot_stack_top;



/**
 *  @brief: kernel总入口
 */
void main( void ) {

    load_app(); // 加载app到指定内存地址
    trap_init(); // 重定向trap入口地址
    task_manager_init(); // 初始化TaskManager
    timer_init(); // 初始化timer
    set_next_trigger(); // 设置时间片

    printk("[kernel] init ok\n");

    run_first_app();

    panic("panic");
}
