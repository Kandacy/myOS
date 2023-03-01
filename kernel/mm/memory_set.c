
/**
 *  @author: Song XiuMing
 *  @date: 22/11/24
 */


#include "mm/mem.h"
#include "type.h"
#include "tools/elf_decode.h"



/* .ld */
extern void text_start();
extern void text_end();
extern void rodata_start();
extern void rodata_end();
extern void data_start();
extern void data_end();
extern void bss_start();
extern void bss_end();
extern void ekernel();
extern void strampoline(); // trap跳板位置




/**
 *  @brief: 初始化MapArea
 *  @param:
 *      ma: MapArea指针
 *      start_va: 逻辑段起始VA
 *      end_va: 逻辑段中止VA
 *      mt: 逻辑段类型
 *      mp: 逻辑段权限
 *  @return: 
 */
void map_area_init(MapArea* ma, VirtAddr start_va, VirtAddr end_va, MapType mt, MapPermission mp){
    ma->vpn_range.start_vpn = va_to_vpn(start_va, ADDR_FLOOR);
    ma->vpn_range.size = va_to_vpn(end_va, ADDR_CEIL) - ma->vpn_range.start_vpn;
    ma->map_type = mt;
    ma->map_premission = mp;
}




// /**
//  *  @brief: 复制页面（此函数有错误，停用！）
//  *  @param:
//  *      ppn: 目的物理地址页号
//  *      vpn: 源虚拟地址页号
//  *  @return: 
//  */
// void map_area_page_copy(PhysPageNum ppn, VirtPageNum vpn){
//     u8* pa = (u8*)ppn_to_pa(ppn);
//     u8* va = (u8*)vpn_to_va(vpn);
//     for(i64 i = 0; i < PAGE_SIZE; i ++){
//         *pa = *va;
//     }
// }




/**
 *  @brief: 建立逻辑段页表映射
 *  @param:
 *      ma: MapArea指针
 *      pt: 控制页表目录项
 *  @return: 
 */
void map_area_map(MapArea* ma, PageTable* pt){
    PhysPageNum target;
    for(i64 i = 0; i < ma->vpn_range.size; i ++){
        /* 判断是何种映射方式，从而得到 target PPN */
        if (ma->map_type == MAP_TYPE_IDENTICAL) {  // 恒等映射
            target = (PhysPageNum)ma->vpn_range.start_vpn + i;
        } else {  // 申请新页面
            target = frame_allocator_alloc();
        }
        // 建立PPN和VPN的映射
        pt_map_ppn_vpn(pt, target, ma->vpn_range.start_vpn + i, ma->map_premission);
    }
}




/* vector初始化相关函数 */
//比较函数
i64 vMA_cmp(const void* elemAddr1,const void* elemAddr2){
    return (i64)((MapArea*)elemAddr1)->vpn_range.start_vpn - (i64)((MapArea*)elemAddr2)->vpn_range.start_vpn;
}


/**
 *  @brief: 初始化MemorySet
 *  @param:
 *      ms: MemorySet指针
 *  @return: 
 */
void memory_set_init(MemorySet* ms){
    ms->page_table = page_table_new();
    vector_init(&ms->areas, sizeof(MapArea), NULL, NULL, vMA_cmp);
}




/**
 *  @brief: 将逻辑段加入地址空间
 *  @param:
 *      ms: 地址空间
 *      ma: 逻辑段
 *  @return: 
 */
void memory_set_push(MemorySet* ms, MapArea* ma){
    vector_push(&ms->areas, ma); // 将MA插入MS中
    // 将逻辑段建立映射
    map_area_map(ma, &ms->page_table);
}




/**
 *  @brief: 建立跳板页面地址映射
 *  @param:
 *      ms: 要建立映射的地址空间
 *  @return: 
 */
void memory_set_map_trampoline(MemorySet *ms){
    // printk("[memory_set.c] strampoline = 0x%x, TRAMPOLINE = 0x%lx\n", (u64)strampoline, (u64)TRAMPOLINE_FULL);
    pt_map_ppn_vpn(&ms->page_table, 
                    pa_to_ppn((PhysAddr)strampoline, ADDR_FLOOR), 
                    va_to_vpn(TRAMPOLINE_FULL, ADDR_FLOOR),
                    PTE_FLAG_BIT_R | PTE_FLAG_BIT_X);
}




/**
 *  @brief: 建立用户栈地址映射
 *  @param:
 *      ms: 要建立映射的地址空间
 *  @return: 
 */
void memory_set_map_user_stack(MemorySet *ms, VirtAddr user_stack_low_va){
    // printk("[memory_set.c] strampoline = 0x%x, TRAMPOLINE = 0x%lx\n", (u64)strampoline, (u64)TRAMPOLINE);
    PhysPageNum user_stack_ppn = frame_allocator_alloc();
    pt_map_ppn_vpn(&ms->page_table, 
                    user_stack_ppn, 
                    va_to_vpn(user_stack_low_va, ADDR_FLOOR),
                    PTE_FLAG_BIT_R | PTE_FLAG_BIT_W | PTE_FLAG_BIT_U);
}




/**
 *  @brief: 建立用户内核栈地址映射（分配新页作为内核栈）
 *  @param:
 *      ms: 要建立映射的地址空间
 *  @return: 
 */
void memory_set_map_kernel_stack(MemorySet *ms){
    PhysPageNum kernel_stack_ppn = frame_allocator_alloc();
    // printk("[memory_set.c] kernel_stack_ppn = 0x%lx, \n", (u64)kernel_stack_ppn);
    pt_map_ppn_vpn(&ms->page_table, 
                    kernel_stack_ppn, 
                    va_to_vpn(TRAP_CONTEXT_FULL, ADDR_FLOOR),
                    PTE_FLAG_BIT_R | PTE_FLAG_BIT_W);
}




/**
 *  @brief: 建立内核内核栈地址映射
 *  @param:
 *      ms: 要建立映射的地址空间
 *  @return: 
 */
void memory_set_map_kernel_stack_kernel(MemorySet *ms){
    PhysPageNum kernel_stack_ppn = frame_allocator_alloc();
    // printk("[memory_set.c] kernel_stack_ppn = 0x%lx, \n", (u64)kernel_stack_ppn);
    pt_map_ppn_vpn(&ms->page_table, 
                    kernel_stack_ppn, 
                    va_to_vpn(TRAP_CONTEXT_FULL, ADDR_FLOOR),
                    PTE_FLAG_BIT_R | PTE_FLAG_BIT_W);
}




/**
 *  @brief: 建立kernel的地址空间
 *  @param:
 *      ms: 地址空间
 *  @return: 
 */
void memory_set_kernel_new(MemorySet* ms){
    MapArea ma_tmp;
    printk("[mm/memory_set.c] text: 0x%x - 0x%x\n", (u64)text_start, (u64)text_end);
    printk("[mm/memory_set.c] rodata: 0x%x - 0x%x\n", (u64)rodata_start, (u64)rodata_end);
    printk("[mm/memory_set.c] data: 0x%x - 0x%x\n", (u64)data_start, (u64)data_end);
    printk("[mm/memory_set.c] bss: 0x%x - 0x%x\n", (u64)bss_start, (u64)bss_end);
    memory_set_map_trampoline(ms); // 映射跳板
    memory_set_map_kernel_stack_kernel(ms); // 映射内核栈（用于trap时直接访问栈，而不用改变sp）
    map_area_init(&ma_tmp, (VirtAddr)text_start,   (VirtAddr)text_end,   MAP_TYPE_IDENTICAL, MAP_PREMISSION_R | MAP_PREMISSION_X);
    memory_set_push(ms, &ma_tmp);
    map_area_init(&ma_tmp, (VirtAddr)rodata_start, (VirtAddr)rodata_end, MAP_TYPE_IDENTICAL, MAP_PREMISSION_R                   );
    memory_set_push(ms, &ma_tmp);
    map_area_init(&ma_tmp, (VirtAddr)data_start,   (VirtAddr)data_end,   MAP_TYPE_IDENTICAL, MAP_PREMISSION_R | MAP_PREMISSION_W);
    memory_set_push(ms, &ma_tmp);
    map_area_init(&ma_tmp, (VirtAddr)bss_start,    (VirtAddr)bss_end,    MAP_TYPE_IDENTICAL, MAP_PREMISSION_R | MAP_PREMISSION_W);
    memory_set_push(ms, &ma_tmp);
    map_area_init(&ma_tmp, (VirtAddr)ekernel,      (VirtAddr)MEMORY_END, MAP_TYPE_IDENTICAL, MAP_PREMISSION_R | MAP_PREMISSION_W);
    memory_set_push(ms, &ma_tmp);
}




/**
 *  @brief: 建立task的地址空间
 *  @param:
 *      ms: task的地址空间
 *      va_start: task起始位置
 *      va_end: task最大结束位置
 *      user_stack_low: 用户栈低虚拟地址
 *  @return: 
 */
void memory_set_task_init_identical(MemorySet* ms, VirtAddr va_start, VirtAddr va_end, VirtAddr user_stack_low){
    MapArea ma_tmp;
    memory_set_map_trampoline(ms); // 映射trap跳板
    memory_set_map_user_stack(ms, user_stack_low); // 映射用户栈
    memory_set_map_kernel_stack(ms); // 映射用户页表的内核栈，用于保存上下文
    // 将task所有部分都归于一个段，赋全部权限
    map_area_init(&ma_tmp, va_start, user_stack_low - 1, MAP_TYPE_IDENTICAL, MAP_PREMISSION_R | MAP_PREMISSION_W | MAP_PREMISSION_X | MAP_PREMISSION_U);
    memory_set_push(ms, &ma_tmp);
}




/**
 *  @brief: 建立task的地址空间
 *  @param:
 *      elf_data: elf头文件
 *  @return: 
 */
void memory_set_from_elf(MemorySet* ms, u8* elf_data){

    Elf64_Ehdr* elf = (Elf64_Ehdr*)elf_data; // elf头
    
    Elf64_Phdr* phdr = (Elf64_Phdr*)(elf_data + elf->e_phoff); // 找到段头表的位置
    u64 phdr_size = elf->e_phentsize; // 一个段头的size 
    u64 phdr_num = elf->e_phnum; // 有多少个段

    VirtPageNum target_vaddr;
    u64 target_size;
    MapPermission mp_flags;
    MapArea ma_tmp;

    memory_set_map_trampoline(ms); // 映射跳板

    for(i64 i = 0; i < phdr_num; i ++){ // 将所有段建立映射

        if (phdr->p_type != PT_LOAD) { // 该段没有被加载的必要，跳过
            continue;
        }

        target_vaddr = phdr->p_vaddr; // 确定该段起始虚拟地址
        target_size = phdr->p_memsz; // 确定该段大小

        // 确定该段权限
        mp_flags = MAP_PREMISSION_U;
        if (phdr->p_flags & PF_X) {mp_flags |= MAP_PREMISSION_X;}
        if (phdr->p_flags & PF_R) {mp_flags |= MAP_PREMISSION_R;}
        if (phdr->p_flags & PF_W) {mp_flags |= MAP_PREMISSION_W;}

        // 将参数写入相应逻辑段
        // 用户程序在链接是就已经按照逻辑段进行页面对齐了，所以无需复制内存
        map_area_init(&ma_tmp, target_vaddr, target_vaddr + target_size, MAP_TYPE_FRAMED, mp_flags);
        memory_set_push(ms, &ma_tmp);

        // todo: 处理用户栈


        // 定位到下一个phdr
        phdr = (Elf64_Phdr*)((u8*)phdr + phdr_size);
    }
}



