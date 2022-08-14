
// store fault


#include "lib/stdio.h"
#include "user_lib.h"
#include "type.h"


i64 main( void ){
    printf("01t store fault\n");
    u8 *p;
    *p = 0;
    printf("store fault is execed\n");
    return 0;
}
