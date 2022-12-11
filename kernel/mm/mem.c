
#include "mm/mem.h"
#include "type.h"


u8 kernel_stack[APP_NUM_MAX][KERNEL_STACK_SIZE];
u8 user_stack[APP_NUM_MAX][USER_STACK_SIZE];



u64 get_kernel_stack_top( u64 app_id ) {
    return (u64)(kernel_stack[app_id]) + KERNEL_STACK_SIZE;
}


u64 get_user_stack_top( u64 app_id ) {
    return (u64)(user_stack[app_id]) + USER_STACK_SIZE;
}




