
/**
 *  @author: Song XiuMing
 *  @date: 22/08/11
 */

    .align 2
    .section .entry_function
    .global _start
_start:
    # 初始化sp指针到bss段，向下生长
    la sp, boot_stack_top
    # 跳转到kernel的main
    j main


    # bss段内存
    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 * 16
    .globl boot_stack_top
boot_stack_top:


loop:
    j loop
