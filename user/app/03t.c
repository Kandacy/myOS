
#include "lib/stdio.h"
#include "user_lib.h"
#include "type.h"


i64 main( void ){
    printf("03t yield\n");
    sys_yield();
    printf("03t yield success\n");
    for(u64 i = 0; i < 100000000; i ++){
        if(i % 10000000 == 0){
            printf("03t: %d\n", i / 10000000);
        }
    }
    return 0;
}
