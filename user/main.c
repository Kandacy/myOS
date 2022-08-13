
#include "lib/stdio.h"
#include "user_lib.h"
#include "syscall.h"


extern i64 main( void );


/**
 *  @brief: user_lib总入口
 */
void lib_main( void ) {
    printf("[user_lib] init ok\n");
    printf("[user_lib] second print test.\n");
    sys_exit(main());
}



