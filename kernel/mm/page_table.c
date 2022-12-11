
/**
 *  @author: Song XiuMing
 *  @date: 22/08/11
 */



#include "type.h"
#include "mm/mem.h"
#include "lib/vector.h"
#include "lib/stdlib.h"




/**
 *  @brief: 创建页表
 *  @param:
 *  @return: 创建的PageTable
 */
PageTable page_table_new( void ){
    PageTable pt;
    pt.root_ppn = frame_allocator_alloc();
    vector_init(&pt.ppns, sizeof(PhysPageNum), NULL, NULL, vPPN_cmp);
    return pt;
}




/**
 *  @brief: 创建页表项
 *  @param:
 *      ppn: 物理地址
 *      flags: 标志位
 *  @return: 创建PageTableEntry
 */
PageTableEntry pte_new(PhysPageNum ppn, u64 flags) {
    return (PageTableEntry)((ppn << 10) | flags);
}




/**
 *  @brief: PTE提取PA
 *  @param:
 *      pte: 页表项
 *  @return: 页表项中的物理地址
 */
PhysPageNum pte_get_ppn(PageTableEntry pte){
    return (PhysPageNum)(pte >> 10);
}





/**
 *  @brief: 使用VPN，在PT中查找对应的最后一级PTE，没有就新建
 *  @param:
 *      pt: 对应的页表管理项
 *      vpn：要找的虚拟地址
 *  @return: VPN对应最后一级PTE的地址
 */
PageTableEntry *pt_find_pte_from_vpn(PageTable *pt, VirtPageNum vpn){

    u64 vpn_index[3];
    vpn_to_indexes(vpn, vpn_index); // 解析vpn的信息
    // printk("[mm/page_table.c] vpn0 = 0x%x, 1 = 0x%x, 2 = 0x%x\n", (u64)vpn_index[0], (u64)vpn_index[1], (u64)vpn_index[2]);

    PageTableEntry *root_temp, *target;
    root_temp = (PageTableEntry *)ppn_to_pa(pt->root_ppn); // 确定根页表位置
    target = root_temp + vpn_index[0]; // 确定PTE位置

    for(u64 i = 1; i < 3; i ++){ // 只循环两次，指向最后一级页表的PTE
        
        if( (*target)== 0 ){ // 没有下一级，新建
            *target = pte_new(frame_allocator_alloc(), PTE_FLAG_BIT_V);
        }
        target = (PageTableEntry *)ppn_to_pa(pte_get_ppn(*target)) + vpn_index[i]; // 指向下一级
    }

    // printk("[mm/page_table.c] target = 0x%x, *target = 0x%x\n", (u64)target, (u64)(*target));
    return target;
}




/**
 *  @brief: 将物理地址映射到虚拟地址上
 *  @param:
 *      pt: 对应的页表管理项
 *      ppn：物理地址
 *      vpn：虚拟地址
 *      ext_pte_flags：除V位意外的其他标志位
 *  @return: 
 */
void pt_map_ppn_vpn(PageTable *pt, PhysPageNum ppn, VirtPageNum vpn, u64 ext_pte_flag){
    PageTableEntry *pte = pt_find_pte_from_vpn(pt, vpn); // 找到对应PTE
    *pte = pte_new(ppn, PTE_FLAG_BIT_V | ext_pte_flag); // 建立映射
    // printk("[mm/page_table.c] vpn -> pte = 0x%x | 0x%x\n", *pte >> 10, *pte & 0xff);
}




/**
 *  @brief: 拆除物理地址到虚拟地址的映射
 *  @param:
 *      pt: 对应的页表管理项
 *      vpn：虚拟地址
 *  @return: 
 */
void pt_unmap_ppn_vpn(PageTable *pt, VirtPageNum vpn){
    PageTableEntry *pte = pt_find_pte_from_vpn(pt, vpn); // 找到对应PTE
    *pte = pte_new(0, 0); // 拆除映射
}
