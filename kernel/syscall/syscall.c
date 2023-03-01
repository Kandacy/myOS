
#include "syscall/syscall.h"
#include "syscall_id.h"
#include "lib/error.h"
#include "lib/stdio.h"



/**
 *  @brief: user syscall的服务函数
 *  @param:
 *      id: syscall id
 *      argx: 传入的参数
 *  @return: syscall函数对应返回值
 */
u64 syscall(u64 id, u64 arg0, u64 arg1, u64 arg2) {
    u64 ret;
    switch (id) {
        case SYSCALL_WRITE:
            ret = sys_write(arg0, (char *)arg1, arg2);
            break;
        case SYSCALL_EXIT:
            sys_exit(arg0);
            break;
        case SYSCALL_YIELD:
            sys_yield();
            break;
        default:
            printk("[syscall] error id = %x\n", id);
            panic("kernel: syscall id undefined.");
            break;
    }
    return ret;
}
