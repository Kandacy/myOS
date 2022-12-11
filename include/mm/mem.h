
#ifndef __MEM_H
#define __MEM_H

#include "type.h"
#include "config.h"



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
#define PTE_FLAG_BIT_V  (1 << 0)  // 页表项有效？
#define PTE_FLAG_BIT_R  (1 << 1)  // 可读？
#define PTE_FLAG_BIT_W  (1 << 2)  // 可写？
#define PTE_FLAG_BIT_X  (1 << 3)  // 可执行？
#define PTE_FLAG_BIT_U  (1 << 4)  // U模式有权访问？
#define PTE_FLAG_BIT_G  (1 << 5)  // ？
#define PTE_FLAG_BIT_A  (1 << 6)  // 曾被读过？
#define PTE_FLAG_BIT_D  (1 << 7)  // 曾被写过？


// 物理内存中止地址
#define MEMORY_END  0x80800000




typedef u64 PhysAddr;
typedef u64 VirtAddr;
typedef u64 PhysPageNum;
typedef u64 VirtPageNum;

typedef u64 PageTableEntry;




/* mm/addr.c */
PhysPageNum phys_addr_to_page_num(PhysAddr addr, u8 flag);
PhysAddr phys_page_num_to_addr(PhysPageNum ppn);
VirtPageNum virt_addr_to_page_num(VirtAddr addr, u8 flag);
VirtAddr virt_page_num_to_addr(VirtPageNum vpn);
void vpn_to_indexes(VirtPageNum vpn, u64 *indexes);
PageTableEntry *get_pte_array_from_ppn(PhysPageNum ppn);
u8 *get_u8_array_from_ppn(PhysPageNum ppn);



/* mm/page_table.c */
PageTableEntry pte_new(PhysPageNum ppn, u64 flags);



/* mm/frame_allocator.c */
void frame_allocator_init( void );
PhysPageNum frame_allocator_alloc( void );
void frame_allocator_dealloc(PhysPageNum ppn);


#endif /* __MEM_H */
