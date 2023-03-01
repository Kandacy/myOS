
#include "mm/mem.h"
#include "type.h"

#include "driver/riscv.h"


/* 映射类型 */
#define MAP_TYPE_IDENTICAL  0  // 恒等映射
#define MAP_TYPE_FRAMED     1  // 新建页面映射


// 负责管理kernel的地址空间
MemorySet kernel_ms;


u8 kernel_stack[APP_NUM_MAX][KERNEL_STACK_SIZE];
u8 user_stack[APP_NUM_MAX][USER_STACK_SIZE];




u64 get_kernel_stack_top( u64 app_id ) {
    return (u64)(kernel_stack[app_id]) + KERNEL_STACK_SIZE;
}


u64 get_user_stack_top( u64 app_id ) {
    return (u64)(user_stack[app_id]) + USER_STACK_SIZE;
}




/* vector初始化相关函数 */
//比较函数
i64 vPPN_cmp(const void* elemAddr1,const void* elemAddr2) {
    return *(PhysPageNum *)elemAddr1 - *(PhysPageNum *)elemAddr2;
}




// void test(){
//     printk("[mem.c] *0x80201000 = 0x%x\n", *((u64*)0x80201000));
//     printk("[mem.c] *0x7ffffff000 = 0x%x\n", *((u8*)0xffffffffFFFFf000));
// }

/**
 *  @brief: 初始化MMU
 *  @param:
 *  @return: 
 */
void mm_init( void ){
    frame_allocator_init();
    // kernel_mm_set_for_mmu_init();
    memory_set_init(&kernel_ms);
    memory_set_kernel_new(&kernel_ms);
    w_satp( ((u64)0x8 << 60) | (u64)kernel_ms.page_table.root_ppn);
    asm volatile("sfence.vma");
    printk("[mm/mem.c] mm_init ok\n");
}






/* 纪念写错satp导致的bug，特此保留debug用的其中一个函数（虽然这个函数测不出bug */
#if 0
void mm_map_test(PhysPageNum root_ppn, VirtPageNum vpn){
    u64 vpn_index[3];
    PageTableEntry *target;
    for(i64 i = 2; i >= 0; i --){
        vpn_index[i] = vpn & 0x1ff;
        vpn >>= 9;
    }
    target = (PageTableEntry *)ppn_to_pa(root_ppn) + vpn_index[0];
    for(i64 i = 1; i < 3; i ++){
        target = (PageTableEntry *)ppn_to_pa(pte_get_ppn(*target)) + vpn_index[i];
    }
    // printk("[mm/mem.c] test target pte = 0x%x\n", *target >> 10);
}

// 调用测试函数
mm_map_test(kernel_pt.root_ppn, 0x80200);
mm_map_test(kernel_pt.root_ppn, 0x80201);
mm_map_test(kernel_pt.root_ppn, 0x80202);
mm_map_test(kernel_pt.root_ppn, 0x80222);
#endif
