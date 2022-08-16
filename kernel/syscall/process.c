
#include "type.h"
#include "lib/error.h"
#include "lib/stdio.h"
#include "syscall_id.h"
#include "task/task.h"



/**
 *  @brief: user syscall printf服务函数
 *  @param:
 *      fd: 文件打开类型
 *      buf: 要操作的字符串
 *      len: 字符串的长度
 *  @return: 字符串长度
 */
u64 sys_write(u64 fd, char *buf, u64 len) {
    switch (fd) {
        case FD_STDOUT: // 向控制台输出
            printk("%s", buf);
            break;
        default:
            printk("[k] fd = %d\n", fd);
            panic("kernel: sys_write fd undefined.");
            break;
    }
    return len;
}




/**
 *  @brief: user syscall exit服务函数
 *  @param:
 *      exit_id: app退出代码
 *  @return: 
 */
void sys_exit(u64 exit_id) {
    printk("[kernel] app exit %d.\n", exit_id);
    exit_current_app();
}




/**
 *  @brief: user syscall yield服务函数
 *  @param:
 *  @return: 
 */
void sys_yield( void ) {
    // printk("[kernel] app exit %d.\n", exit_id);
    suspend_current_app();
}
