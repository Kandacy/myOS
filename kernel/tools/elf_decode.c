
#include "tools/elf_decode.h"

#include "lib/stdio.h"
#include "lib/error.h"




void elf_head_check(Elf64_Ehdr *elf){
    // 判magic num
    if( (elf->e_ident[0] != 0x7f) || (elf->e_ident[1] != 0x45) || (elf->e_ident[2] != 0x4c) || (elf->e_ident[3] != 0x46) ){
        // printk("[loader.c] 0x%x, 0x%x, 0x%x, 0x%x\n", elf->e_ident[0], elf->e_ident[1], elf->e_ident[2], elf->e_ident[3]);
        panic("error: elf file magic num error.");
    }
    // 判是否为64位
    if( elf->e_ident[EI_CLASS] != ELFCLASS64 ){
        panic("error: elf file is not 64 bit.");
    }
    // 判是否为小端模式
    if( elf->e_ident[EI_DATA] != ELFDATA2LSB ){
        panic("error: elf file is not LSB.");
    }
    // 判是否为可执行文件
    if( elf->e_type != ET_EXEC ){
        panic("error: elf file is not exec file.");
    }
}





void get_elf_data(Elf64_Ehdr *elf){

    elf_head_check();

    u64 entry = elf->e_entry; // 程序入口地址
    u64 eh_size = elf->e_ehsize; // elf头大小
    u64 phnum = elf->e_phnum; // 段头表的个数
    printk("[loader.c] e_entry = 0x%x, phoff = 0x%x\n", entry, elf->e_phoff);

    Elf64_Phdr *ph = (Elf64_Phdr *)((u8 *)elf + eh_size); // 首个段头表的位置
    printk("[loader.c] ph = 0x%x\n", (u64)ph);

    u64 seg_flag;
    PhysAddr seg_start_addr;
    PhysAddr seg_size;
    // 解析每一个段头表
    for(u64 i = 0; i < phnum; i ++){

        printk("[loader.c] ph_flag = 0x%x\n", (u64)ph->p_flags);

        if(ph->p_type == PT_LOAD){ // 我们只装载需要被装载的段

            /* 检测段属性（RWX） */
            seg_flag = PTE_FLAG_BIT_U; // U模式有权访问
            if(ph->p_flags & PF_X){ // 可执行
                seg_flag |= PTE_FLAG_BIT_X;
            }
            if(ph->p_flags & PF_W){ // 可写
                seg_flag |= PTE_FLAG_BIT_W;
            }
            if(ph->p_flags & PF_R){ // 可读
                seg_flag |= PTE_FLAG_BIT_R;
            }

            // 段起始位置
            seg_start_addr = (PhysAddr)ph->p_vaddr;
            seg_end_addr = (PhysAddr)((u8 *)seg_start_addr + ph->p_memsz);
        }

        // ph指向下一个段头表
        ph ++;
    }
}