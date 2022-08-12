
#include "lib/error.h"
#include "lib/stdio.h"
#include "sbi/sbi.h"


/**
 *  @brief: 不可恢复错误，打印出错信息，关机
 *  @param:
 *      s: panic的原因
 *  @return: 
 */
void panic(char *s) {
    printk("[Panic] %s\n", s);
    sbi_shutdown(1);
}
