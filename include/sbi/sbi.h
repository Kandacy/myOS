
/**
 *  @author: Song XiuMing
 *  @date: 22/08/11
 */



#ifndef __SBI_H
#define __SBI_H


#include "type.h"


u8 sbi_console_putchar(u8 ch);
void sbi_shutdown(u8 exit_code);


#endif /* __SBI_H */
