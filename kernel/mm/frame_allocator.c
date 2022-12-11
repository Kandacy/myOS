
#include "type.h"
#include "mm/mem.h"
#include "lib/vector.h"
#include "lib/error.h"
#include "lib/stdio.h"


// ld文件中，kernel中止地址
extern void ekernel(void);



/* 唯一全局变量，用于管理物理页 */
static struct {
    PhysPageNum current; // 未分配物理页帧起始页号
    PhysPageNum end; // 未分配物理页帧结束页号
    vector recycled; // 回收的已使用的物理页
}FrameAllocator;




/**
 *  @brief: 初始化FrameAllocator
 *  @param:
 *  @return: 
 */
void frame_allocator_init( void ){
    FrameAllocator.current = phys_addr_to_page_num((PhysAddr)ekernel, ADDR_CEIL);
    FrameAllocator.end = phys_addr_to_page_num(MEMORY_END, ADDR_FLOOR);
    FrameAllocator.recycled = vector_init(10);
}




/**
 *  @brief: 分配物理页，并将页内清零
 *  @param:
 *  @return: 分配的ppn
 */
PhysPageNum frame_allocator_alloc( void ){

    /* 选择分配的ppn */
    PhysPageNum ret;
    if(FrameAllocator.recycled.used_size == 0){ // 没有回收的ppn
        ret = FrameAllocator.current ++;
    } else {
        ret = vector_pop(&FrameAllocator.recycled);
    }

    /* 页内清零 */
    u8 *pa = (u8 *)phys_page_num_to_addr(ret);
    // printk("[Test] pa = 0x%x\n", (u64)pa);
    for(u64 i = 0; i < PAGE_SIZE; i ++, pa ++){
        *pa = 0;
    }

    return ret;
}




/**
 *  @brief: 回收物理页
 *  @param:
 *      ppn: 回收的ppn
 *  @return: 
 */
void frame_allocator_dealloc(PhysPageNum ppn){
    if(ppn >= FrameAllocator.current || vector_find_elem(FrameAllocator.recycled, ppn) != FrameAllocator.recycled.used_size){ // 该内存未被分配或已被回收
        panic("frame_allocator_dealloc: dealloc unused memory");
    }
    vector_push(&FrameAllocator.recycled, ppn);
}





