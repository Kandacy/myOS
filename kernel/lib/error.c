
#include "lib/error.h"
#include "lib/stdio.h"
#include "sbi/sbi.h"




void assert(u8 isTrue){
    if(isTrue == 0){
        panic("assert error!");
    }
}




/**
 *  @brief: 不可恢复错误，打印出错信息，关机
 *  @param:
 *      s: panic的原因
 *  @return: 
 */
void panic(const char *s) {
    printk("[Panic] %s\n", s);
    sbi_shutdown(1);
}
