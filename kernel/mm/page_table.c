
#include "type.h"
#include "mm/mem.h"
#include "lib/vector.h"



/* 页表管理项，用于保存根页表和已使用的页表 */
typedef struct PageTable{
    PhysPageNum root_ppn; // 根页表位置
    vector ppns; // 保存使用的页表，便于清理
}PageTable;




/**
 *  @brief: 创建PageTable
 *  @param:
 *  @return: 创建的PageTable
 */
PageTable page_table_new( void ){
    PageTable pt;
    pt.root_ppn = frame_allocator_alloc();
    pt.ppns = vector_init(10);
    return pt;
}









/**
 *  @brief: 创建PageTableEntry
 *  @param:
 *      ppn: 物理地址
 *      flags: 标志位
 *  @return: 创建PageTableEntry
 */
PageTableEntry pte_new(PhysPageNum ppn, u64 flags) {
    return (PageTableEntry)((ppn << 10) | flags);
}
