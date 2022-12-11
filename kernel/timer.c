

#include "timer.h"
#include "sbi/sbi.h"
#include "driver/riscv.h"
#include "config.h"
#include "lib/stdio.h"



/**
 *  @brief: 设置下一个时间片
 *  @param:
 *  @return: 
 */
void set_next_trigger( void ) {
    sbi_set_timer(r_time() + CLOCK_FREQ / TICKS_PER_SEC);
}




/**
 *  @brief: 使能sie的timer中断
 *  @param:
 *  @return: 
 */
void timer_init( void ) {
    w_sie(SIE_STIE);
    // printk("[Test] timer: mie = 0x%x\n", r_mie());
}
