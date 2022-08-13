
/**
 *  @author: Song XiuMing
 *  @date: 22/08/11
 */


// #include "start.s"
#include "type.h"
#include "sbi/sbi.h"
#include "lib/stdio.h"
#include "lib/error.h"
#include "lib/string.h"
#include "trap/trap.h"
#include "loader.h"


// extern void boot_stack(void);
// extern void boot_stack_top(void);
// extern u8 *boot_stack;
// extern u8 *boot_stack_top;



/**
 *  @brief: kernel总入口
 */
int main( void ) {

    const char *s = "hello world";
    printk("%s/%d\n", s, strlen(s));

    trap_init();
    load_app();
    run_app();

    panic("panic");

    return 0;
}
