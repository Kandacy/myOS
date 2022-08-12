
/**
 *  @author: Song XiuMing
 *  @date: 22/08/11
 */


// #include "start.s"
#include "type.h"
#include "sbi/sbi.h"
#include "lib/stdio.h"
#include "lib/error.h"


// extern void boot_stack(void);
// extern void boot_stack_top(void);
// extern u8 *boot_stack;
// extern u8 *boot_stack_top;



/**
 *  @brief: kernel总入口
 */
void main( void ) {
    
    // for(u8 i = 'a'; i < 'z'; i ++){
    //     sbi_console_putchar(i);
    // }

    printk("%d/%s/%c\n", 1, "hello", 'a');

    panic("panic");
}
