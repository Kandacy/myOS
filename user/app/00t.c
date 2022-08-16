
#include "lib/stdio.h"
#include "user_lib.h"
#include "type.h"


i64 main( void ){
    printf("00t yield\n");
    sys_yield();
    printf("00t yield success\n");
    return 0;
}
