
// store fault


#include "lib/stdio.h"
#include "user_lib.h"


i64 main( void ){
    printf("01t store fault\n");
    sys_yield();
    printf("01t store fault...\n");
    return 0;
}
