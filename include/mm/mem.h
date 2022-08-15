
#ifndef __MEM_H
#define __MEM_H

#include "type.h"
#include "config.h"



#define KERNEL_STACK_SIZE 4096
#define USER_STACK_SIZE   4096


extern u8 kernel_stack[APP_NUM_MAX][KERNEL_STACK_SIZE];
extern u8 user_stack[APP_NUM_MAX][USER_STACK_SIZE];


u64 get_kernel_stack_top( u64 app_id );
u64 get_user_stack_top( u64 app_id );


#endif /* __MEM_H */
