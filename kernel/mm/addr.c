
#include "type.h"
#include "mm/mem.h"




PhysPageNum pa_to_ppn(PhysAddr addr, u8 flag) {
    return (flag == ADDR_FLOOR) ? (addr >> PAGE_SIZE_BITS) : ((addr >> PAGE_SIZE_BITS) + 1);
}


PhysAddr ppn_to_pa(PhysPageNum ppn) {
    return ppn << PAGE_SIZE_BITS;
}


VirtPageNum va_to_vpn(VirtAddr addr, u8 flag) {
    return (flag == ADDR_FLOOR) ? (addr >> PAGE_SIZE_BITS) : ((addr >> PAGE_SIZE_BITS) + 1);
}


VirtAddr vpn_to_va(VirtPageNum vpn) {
    return vpn << PAGE_SIZE_BITS;
}




/**
 *  @brief: 将vpn转换为三段页表内索引（一段9bit）
 *  @param:
 *      vpn: vpn
 *      indexes: 返回的下标，0存放一级下标，2存放三级下标
 *  @return: 
 */
void vpn_to_indexes(VirtPageNum vpn, u64 *indexes){
    for(i8 i = 2; i >= 0; i --){
        indexes[i] = vpn & 0x1ff;
        vpn >>= 9;
    }
}




/**
 *  @brief: 将页表项作为pte数组返回
 *  @param:
 *      ppn: ppn
 *  @return: pte数组起始地址
 */
PageTableEntry *get_pte_array_from_ppn(PhysPageNum ppn){
    return (PageTableEntry *)ppn_to_pa(ppn);
}




/**
 *  @brief: 将页表项作为u8数组返回
 *  @param:
 *      ppn: ppn
 *  @return: pte数组起始地址
 */
u8 *get_u8_array_from_ppn(PhysPageNum ppn){
    return (u8 *)ppn_to_pa(ppn);
}



