
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

    for(u64 i = 0; i < num_app; i ++){
        u8 *app_addr;
        app_addr = (u8 *)(APP_BASE_ADDRESS + APP_MAX_SIZE * i);
        printk("[loader.c] loader: app 0x%x - 0x%x\n", num_app_ptr[i], num_app_ptr[i + 1]);

        // 将app代码放到内存中
        for(u8 *target = (u8 *)num_app_ptr[i]; (u64)target <= (u64)num_app_ptr[i + 1]; target ++, app_addr ++){
            *app_addr = *target;
        }
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




