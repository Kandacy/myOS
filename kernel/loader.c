
/**
 *  加载app到指定的内存地址
 */


#include "lib/stdio.h"
#include "trap/trap.h"
#include "driver/riscv.h"
#include "lib/error.h"
#include "mm/mem.h"
#include "config.h"



extern void _num_app(void);


#define EI_NIDENT (16)


/* ELF文件头结构 */
typedef struct
{
  u8	e_ident[EI_NIDENT]; /* Magic number and other info */       // 1 byte * 16
  u16	e_type;		    /* Object file type */                  // 2 bytes
  u16	e_machine;	    /* Architecture */
  u32	e_version;	    /* Object file version */               // 4 bytes
  u64	e_entry;	    /* Entry point virtual address */       // 8 bytes
  u64	e_phoff;	    /* Program header table file offset */  // 8 bytes
  u64	e_shoff;	    /* Section header table file offset */      
  u32	e_flags;	    /* Processor-specific flags */
  u16	e_ehsize;	    /* ELF header size in bytes */
  u16	e_phentsize;	/* Program header table entry size */
  u16	e_phnum;	    /* Program header table entry count */
  u16	e_shentsize;	/* Section header table entry size */
  u16	e_shnum;	    /* Section header table entry count */
  u16	e_shstrndx;	    /* Section header string table index */
} Elf64_Ehdr;


#define EI_CLASS	4		/* File class byte index */
#define ELFCLASSNONE	0		/* Invalid class */
#define ELFCLASS32	1		/* 32-bit objects */
#define ELFCLASS64	2		/* 64-bit objects */
#define ELFCLASSNUM	3

#define EI_DATA		5		/* Data encoding byte index */
#define ELFDATANONE	0		/* Invalid data encoding */
#define ELFDATA2LSB	1		/* 2's complement, little endian */
#define ELFDATA2MSB	2		/* 2's complement, big endian */
#define ELFDATANUM	3

/* Legal values for e_type (object file type).  */
#define ET_NONE		0		/* No file type */
#define ET_REL		1		/* Relocatable file */
#define ET_EXEC		2		/* Executable file */
#define ET_DYN		3		/* Shared object file */
#define ET_CORE		4		/* Core file */
#define	ET_NUM		5		/* Number of defined types */
#define ET_LOOS		0xfe00		/* OS-specific range start */
#define ET_HIOS		0xfeff		/* OS-specific range end */
#define ET_LOPROC	0xff00		/* Processor-specific range start */
#define ET_HIPROC	0xffff		/* Processor-specific range end */



typedef struct
{
  u32	p_type;			/* Segment type */               // 4 bytes
  u32	p_flags;		/* Segment flags */
  u64	p_offset;		/* Segment file offset */          // 8 bytes
  u64	p_vaddr;		/* Segment virtual address */    // 8 bytes
  u64	p_paddr;		/* Segment physical address */
  u64	p_filesz;		/* Segment size in file */       // 8 bytes
  u64	p_memsz;		/* Segment size in memory */
  u64	p_align;		/* Segment alignment */
} Elf64_Phdr;


/* Legal values for p_type (segment type).  */
#define	PT_NULL		0		 /* Program header table entry unused */
#define PT_LOAD		1		 /* Loadable program segment */
#define PT_DYNAMIC	2		 /* Dynamic linking information */
#define PT_INTERP	3		 /* Program interpreter */
#define PT_NOTE		4		 /* Auxiliary information */
#define PT_SHLIB	5		 /* Reserved */
#define PT_PHDR		6		 /* Entry for header table itself */

/* Legal values for p_flags (segment flags).  */
#define PF_X		(1 << 0)	/* Segment is executable */
#define PF_W		(1 << 1)	/* Segment is writable */
#define PF_R		(1 << 2)	/* Segment is readable */
#define PF_MASKOS	0x0ff00000	/* OS-specific */
#define PF_MASKPROC	0xf0000000	/* Processor-specific */






// void get_elf_data(Elf64_Ehdr *elf){

//     // 判magic num
//     if( (elf->e_ident[0] != 0x7f) || (elf->e_ident[1] != 0x45) || (elf->e_ident[2] != 0x4c) || (elf->e_ident[3] != 0x46) ){
//         // printk("[loader.c] 0x%x, 0x%x, 0x%x, 0x%x\n", elf->e_ident[0], elf->e_ident[1], elf->e_ident[2], elf->e_ident[3]);
//         panic("error: elf file magic num error.");
//     }
//     // 判是否为64位
//     if( elf->e_ident[EI_CLASS] != ELFCLASS64 ){
//         panic("error: elf file is not 64 bit.");
//     }
//     // 判是否为小端模式
//     if( elf->e_ident[EI_DATA] != ELFDATA2LSB ){
//         panic("error: elf file is not LSB.");
//     }
//     // 判是否为可执行文件
//     if( elf->e_type != ET_EXEC ){
//         panic("error: elf file is not exec file.");
//     }

//     u64 entry = elf->e_entry; // 程序入口地址
//     u64 eh_size = elf->e_ehsize; // elf头大小
//     u64 phnum = elf->e_phnum; // 段头表的个数
//     printk("[loader.c] e_entry = 0x%x, phoff = 0x%x\n", entry, elf->e_phoff);

//     Elf64_Phdr *ph = (Elf64_Phdr *)((u8 *)elf + eh_size); // 首个段头表的位置
//     printk("[loader.c] ph = 0x%x\n", (u64)ph);

//     u64 seg_flag;
//     PhysAddr seg_start_addr;
//     PhysAddr seg_size;
//     // 解析每一个段头表
//     for(u64 i = 0; i < phnum; i ++){

//         printk("[loader.c] ph_flag = 0x%x\n", (u64)ph->p_flags);

//         if(ph->p_type == PT_LOAD){ // 我们只装载需要被装载的段

//             /* 检测段属性（RWX） */
//             seg_flag = PTE_FLAG_BIT_U; // U模式有权访问
//             if(ph->p_flags & PF_X){ // 可执行
//                 seg_flag |= PTE_FLAG_BIT_X;
//             }
//             if(ph->p_flags & PF_W){ // 可写
//                 seg_flag |= PTE_FLAG_BIT_W;
//             }
//             if(ph->p_flags & PF_R){ // 可读
//                 seg_flag |= PTE_FLAG_BIT_R;
//             }

//             // 段起始位置
//             seg_start_addr = (PhysAddr)ph->p_vaddr;
//             seg_end_addr = (PhysAddr)((u8 *)seg_start_addr + ph->p_memsz);

//             // 装载程序到相应页面中
//             mm_map_for_mmu();
//         }

//         // ph指向下一个段头表
//         ph ++;
//     }
// }




/**
 *  @brief: 加载app到0x8040_0000
 *  @param:
 *  @return:
 */
void load_app( void ) {
    
    u64 *num_app_ptr = (u64 *)_num_app;
    // app数量
    u64 num_app = *num_app_ptr;
    num_app_ptr ++; // 现在num_app_ptr是app文件起始位置

    // 刷新缓冲区
    asm volatile("fence.i");

    //  测试elf文件
    // get_elf_data((Elf64_Ehdr *)num_app_ptr[2]);

    for(u64 i = 0; i < num_app; i ++){
        u8 *app_addr;
        app_addr = (u8 *)(APP_BASE_ADDRESS + APP_MAX_SIZE * i);

        // printk("[Test] loader: app 0x%x - 0x%x\n", num_app_ptr[i], num_app_ptr[i + 1]);

        

        // 将app代码放到内存中
        // for(u8 *target = (u8 *)num_app_ptr[i]; (u64)target <= (u64)num_app_ptr[i + 1]; target ++, app_addr ++){
        //     *app_addr = *target;
        // }
    }
}









/* 分节使用这个结构 */
#if 0
/* 节头表 */
typedef struct
{
  u32	sh_name;	 /* Section name (string tbl index) */    // 4 bytes
  u32	sh_type;	 /* Section type */                       
  u64	sh_flags;	 /* Section flags */                      // 8 bytes
  u64	sh_addr;	 /* Section virtual addr at execution */  // 8 bytes
  u64	sh_offset;	 /* Section file offset */                // 8 bytes
  u64	sh_size;	 /* Section size in bytes */
  u32	sh_link;	 /* Link to another section */
  u32	sh_info;	 /* Additional section information */
  u64	sh_addralign;	 /* Section alignment */
  u64	sh_entsize;	 /* Entry size if section holds table */
} Elf64_Shdr;


/* Legal values for sh_type (section type).  */
#define SHT_NULL	  0		/* Section header table entry unused */
#define SHT_PROGBITS  1		/* Program data */
#define SHT_SYMTAB	  2		/* Symbol table */
#define SHT_STRTAB	  3		/* String table */
#define SHT_RELA	  4		/* Relocation entries with addends */
#define SHT_HASH	  5		/* Symbol hash table */
#define SHT_DYNAMIC	  6		/* Dynamic linking information */
#define SHT_NOTE	  7		/* Notes */
#define SHT_NOBITS	  8		/* Program space with no data (bss) */
#define SHT_REL		  9		/* Relocation entries, no addends */
#define SHT_SHLIB	  10		/* Reserved */
#define SHT_DYNSYM	  11		/* Dynamic linker symbol table */

/* Legal values for sh_flags (section flags).  */
#define SHF_WRITE	     (1 << 0)	/* Writable */
#define SHF_ALLOC	     (1 << 1)	/* Occupies memory during execution */
#define SHF_EXECINSTR	     (1 << 2)	/* Executable */
#endif




