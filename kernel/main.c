
/**
 *  @author: Song XiuMing
 *  @date: 22/08/11
 */


// #include "start.s"
#include "driver/riscv.h"
#include "lib/stdio.h"
#include "lib/error.h"
#include "lib/string.h"
#include "lib/stdlib.h"
#include "lib/vector.h"
#include "mm/mem.h"
#include "task/task.h"
#include "trap/trap.h"
#include "loader.h"
#include "timer.h"
#include "type.h"




// 测试malloc和free
void test_malloc_free( void ) {
    u8 *p[6];
    for(u64 i = 1; i <= 5; i ++){
        p[i] = (u8 *)malloc(i * 10);
        printk("[Test] p[%d] = 0x%x\n", i, (u64)p[i]);
    }
    for(u64 i = 1; i <= 5; i ++){
        free(p[i]);
        printk("[Test] p[%d] free\n", i);
    }
    for(u64 i = 1; i <= 5; i ++){
        p[i] = (u8 *)malloc((6 - i) * 10);
        printk("[Test] p[%d] = 0x%x\n", i, (u64)p[i]);
    }
    for(u64 i = 1; i <= 5; i ++){
        free(p[i]);
        printk("[Test] p[%d] free\n", i);
    }
    for(u64 i = 1; i <= 5; i ++){
        p[i] = (u8 *)malloc(i * 10);
        printk("[Test] p[%d] = 0x%x\n", i, (u64)p[i]);
    }
    for(u64 i = 1; i <= 5; i ++){
        free(p[i]);
        printk("[Test] p[%d] free\n", i);
    }
    printk("[Test] malloc free ok\n");
}




// 测试realloc
void test_realloc( void ){
    u8 *p;

    p = (u8 *)malloc(1);
    for(u64 i = 1; i <= 5; i ++){
        p = (u8 *)realloc(p, i * 10);
        printk("[Test] p%d = 0x%x\n", i, (u64)p);
    }
    free(p);

    p = (u8 *)malloc(1);
    for(u64 i = 1; i <= 5; i ++){
        p = (u8 *)realloc(p, i * 10);
        printk("[Test] p%d = 0x%x\n", i, (u64)p);
    }
    free(p);

    p = (u8 *)malloc(10 * sizeof(u8));
    for(u64 i = 1; i <= 5; i ++){
        p[i] = i;
    }
    p = (u8 *)realloc(p, 20 * sizeof(u8));
    for(u64 i = 1; i <= 5; i ++){
        printk("[Test] p%d = 0x%x\n", i, p[i]);
    }
    free(p);
}



#if VECTOR_GENERICITY

#else
// 测试vector
void test_vector( void ){
    vector vec = vector_init(10);
    for(u64 i = 0; i <= 15; i ++){
        vector_push(&vec, i);
    }
    for(u64 i = 0; i <= 15; i ++){
        printk("[Test] vec[%d] = 0x%x\n", i, vec.data[i]);
    }
    for(u64 i = 7; i <= 15; i ++){
        vector_pop(&vec);
    }
    for(u64 i = 0; i <= 15; i ++){
        printk("[Test] vec[%d] = 0x%x\n", i, vector_get_index(vec, i));
    }
    for(u64 i = 0; i <= 15; i ++){
        vector_push(&vec, i);
    }
    for(u64 i = 0; i <= 30; i ++){
        printk("[Test] vec[%d] = 0x%x\n", i, vec.data[i]);
    }
}
#endif



// 测试FrameAllocator
void test_frame_allocator( void ){
    frame_allocator_init();
    printk("[Test] init ok\n");
    PhysPageNum ppn[40];
    for(u64 i = 0; i < 15; i ++){
        ppn[i] = frame_allocator_alloc();
        printk("[Test] ppn[%d] = 0x%x\n", i, ppn[i]);
    }
    for(u64 i = 0; i < 14; i ++){
        frame_allocator_dealloc(ppn[0 + i]);
    }
    for(u64 i = 15; i < 30; i ++){
        ppn[i] = frame_allocator_alloc();
        printk("[Test] ppn[%d] = 0x%x\n", i, ppn[i]);
    }
    for(u64 i = 14; i < 30; i ++){
        frame_allocator_dealloc(ppn[0 + i]);
    }
    for(u64 i = 0; i < 15; i ++){
        ppn[i] = frame_allocator_alloc();
        printk("[Test] ppn[%d] = 0x%x\n", i, ppn[i]);
    }
    for(u64 i = 0; i < 14; i ++){
        frame_allocator_dealloc(ppn[0 + i]);
    }
    for(u64 i = 15; i < 40; i ++){
        ppn[i] = frame_allocator_alloc();
        printk("[Test] ppn[%d] = 0x%x\n", i, ppn[i]);
    }
}






/**
 *  @brief: kernel总入口
 */
void main( void ) {

    trap_init(); // 重定向trap入口地址
    kernel_heap_init(); // 初始化kernel heap（malloc分配部分）
    mm_init(); // 初始化MMU
    load_app(); // 加载app到指定内存地址
    task_manager_init(); // 初始化TaskManager
    timer_init(); // 初始化timer，开sie
    printk("[kernel] init ok\n");

    /* 开始执行程序 */
    set_next_trigger(); // 设置时间片
    run_first_app();


    /* -----------测试程序----------- */
    // test_realloc();
    // test_vector();
    // test_frame_allocator();
    /* ----------------------------- */


    panic("error: kernel main exec finish");
}
