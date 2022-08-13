
#include "syscall.h"
#include "lib/string.h"
#include "../include/syscall_id.h"



/**
 *  @brief: syscall调用接口，进入kernel
 *  @param:
 *      which: 触发ecall的条件，存在x17(a7)中
 *      args:  传入的参数
 *  @return: ecall返回值，保存在x10(a0)中
 */
i64 syscall(u64 id, u64 arg0, u64 arg1, u64 arg2) {
    register u64 a0 asm("a0") = (u64)(arg0); 
    register u64 a1 asm("a1") = (u64)(arg1); 
    register u64 a2 asm("a2") = (u64)(arg2); 
    register u64 a7 asm("a7") = (u64)(id); 
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory"); 
    return (i64)a0; 
}


i64 sys_write(u64 fd, char *buf) {
    return syscall(SYSCALL_WRITE, fd, (u64)buf, strlen(buf));
}


void sys_exit(i32 id) {
    syscall(SYSCALL_EXIT, id, 0, 0);
}
