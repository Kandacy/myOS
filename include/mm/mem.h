
#ifndef __MEM_H
#define __MEM_H

#include "type.h"
#include "config.h"

#include "lib/vector.h"
#include "lib/stdio.h"



//----------------------------------------------------------------

#define KERNEL_STACK_SIZE 4096
#define USER_STACK_SIZE   4096


extern u8 kernel_stack[APP_NUM_MAX][KERNEL_STACK_SIZE];
extern u8 user_stack[APP_NUM_MAX][USER_STACK_SIZE];


u64 get_kernel_stack_top( u64 app_id );
u64 get_user_stack_top( u64 app_id );


//----------------------------------------------------------------



/* 页相关 */
#define PAGE_SIZE_BITS 12
#define PAGE_SIZE      (1 << PAGE_SIZE_BITS)


/* 地址转换至页号的取整方式 */
#define ADDR_FLOOR  0  // 向下取整
#define ADDR_CEIL   1  // 向上取整


/* PTE标志位 */
#define PTE_FLAG_BIT_V  (1)  // 页表项有效？
#define PTE_FLAG_BIT_R  (1 << 1)  // 可读？
#define PTE_FLAG_BIT_W  (1 << 2)  // 可写？
#define PTE_FLAG_BIT_X  (1 << 3)  // 可执行？
#define PTE_FLAG_BIT_U  (1 << 4)  // U模式有权访问？
#define PTE_FLAG_BIT_G  (1 << 5)  // ？
#define PTE_FLAG_BIT_A  (1 << 6)  // 曾被读过？
#define PTE_FLAG_BIT_D  (1 << 7)  // 曾被写过？



typedef u64 PhysAddr;
typedef u64 VirtAddr;
typedef u64 PhysPageNum;
typedef u64 VirtPageNum;

/* 页表管理项，用于保存根页表和已使用的页表 */
typedef struct PageTable{
    PhysPageNum root_ppn; // 根页表页号
    Vector ppns; // 保存使用的页表，便于清理
}PageTable;

typedef u64 PageTableEntry;




i64 vPPN_cmp(const void* elemAddr1,const void* elemAddr2);




/* mm/mem.c */
void mm_init( void );
void mm_map_for_mmu(PageTable *pt, PhysAddr pa_start, PhysAddr pa_end, u64 ext_pte_flags, u64 map_type);



/* mm/addr.c */
PhysPageNum pa_to_ppn(PhysAddr addr, u8 flag);
PhysAddr ppn_to_pa(PhysPageNum ppn);
VirtPageNum va_to_vpn(VirtAddr addr, u8 flag);
VirtAddr vpn_to_va(VirtPageNum vpn);
void vpn_to_indexes(VirtPageNum vpn, u64 *indexes);
PageTableEntry *get_pte_array_from_ppn(PhysPageNum ppn);
u8 *get_u8_array_from_ppn(PhysPageNum ppn);



/* mm/frame_allocator.c */
void frame_allocator_init( void );
PhysPageNum frame_allocator_alloc( void );
void frame_allocator_dealloc(PhysPageNum ppn);



/* mm/page_table.c */
PageTable page_table_new( void );
PageTableEntry pte_new(PhysPageNum ppn, u64 flags);
PhysPageNum pte_get_ppn(PageTableEntry pte);
void pt_map_ppn_vpn(PageTable *pt, PhysPageNum ppn, VirtPageNum vpn, u64 ext_pte_flag);
void pt_unmap_ppn_vpn(PageTable *pt, VirtPageNum vpn);



#endif /* __MEM_H */
