
/**
 *  @author: Song XiuMing
 *  @date: 22/08/11
 */


#include "sbi/sbi.h"



/** sbi调用的id */
#define SBI_SET_TIMER               0
#define SBI_CONSOLE_PUTCHAR         1
#define SBI_CONSOLE_GETCHAR         2
#define SBI_CLEAR_IPI               3
#define SBI_SEND_IPI                4
#define SBI_REMOTE_FENCE_I          5
#define SBI_REMOTE_SFENCE_VMA       6
#define SBI_REMOTE_SFENCE_VMA_ASID  7
#define SBI_SHUTDOWN                8
#define SBI_CONSOLE_PUTSTR          9
#define SBI_SD_WRITE               10
#define SBI_SD_READ                11
#define SBI_FDT_READ               12



/**
 *  @brief: sbi调用接口，进入m-mode
 *  @param:
 *      which: 触发ecall的条件，存在x17(a7)中
 *      argx:  传入的参数
 *  @return: ecall返回值，保存在x10(a0)中
 */
i64 sbi_call(u64 which, u64 arg0, u64 arg1, u64 arg2) { 
    register u64 a0 asm("a0") = (u64)(arg0); 
    register u64 a1 asm("a1") = (u64)(arg1); 
    register u64 a2 asm("a2") = (u64)(arg2); 
    register u64 a7 asm("a7") = (u64)(which); 
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory"); 
    return (i64)a0; 
}



/**
 *  @brief: 调用sbi接口，向控制台输出字符
 *  @param:
 *      ch: 要输出的字符
 *  @return: 传入的ch
 */
u8 sbi_console_putchar(u8 ch) {
    return (u8)sbi_call(SBI_CONSOLE_PUTCHAR, ch, 0, 0);
}


/**
 *  @brief: 调用sbi接口，关机
 *  @param:
 *      exit_code: 退出代码
 *  @return: 
 */
void sbi_shutdown(u8 exit_code) {
    sbi_call(SBI_SHUTDOWN, exit_code, 0, 0);
}


