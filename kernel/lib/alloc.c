
#include "lib/stdlib.h"
#include "type.h"
#include "lib/stdio.h"


#define KERNEL_HEAP_SIZE 0x2000

#define BLOCK_FREE      0
#define BLOCK_NOT_FREE  1


static u8 kernel_heap[KERNEL_HEAP_SIZE] = {0};




typedef struct block_head{
    struct block_head *next; // 指向下一个区块
    struct block_head *prev; // 指向下一个区块
    u64 size; // 区块大小
    u64 is_free; // 此块是否被使用
}block_head;




/**
 *  @brief: 初始化kernel_heap，向头部装入block_head
 *  @param:
 *  @return: 
 */
void kernel_heap_init( void ) {
    block_head *head = (block_head *)kernel_heap;
    head->next = NULL;
    head->prev = NULL;
    head->size = KERNEL_HEAP_SIZE - sizeof(block_head);
    head->is_free = BLOCK_FREE;
    printk("[Test] kernel heap init: kernel_heap = 0x%x\n", (u64)kernel_heap);
}




/**
 *  @brief: 初始化kernel_heap，向头部装入block_head
 *  @param:
 *      size: 要分配的字节数
 *  @return: 指向分配的内存首地址
 */
void *malloc(u64 size) {

    block_head *target = (block_head *)kernel_heap;
    void *target_space = NULL; // 指向要返回的内存位置
    block_head *front = NULL; // 保存target之前的节点

    // 寻找可以分配的空间
    while(target != NULL && (target->is_free == BLOCK_NOT_FREE || target->size < size) ){
        target = target->next;
        // printk("[Test] malloc: target = 0x%x\n", (u64)target);
    }
    if(target == NULL){ // 没有合适的空间
        printk("[Error] malloc: no enough memory\n");
        return NULL;
    }

    // 分配空间
    target_space = (void *)((u64)target + sizeof(block_head));

    // 设置要分配空间的block_head
    target->is_free = BLOCK_NOT_FREE;

    // 设置下一个block_head
    if (target->next != NULL && size >= target->size - sizeof(block_head)) { // target在中间，且不够装下一个block_head
        // 将这一块全分配出去，避免内存泄漏
        // 这里什么都不用做，直接返回即可
        return target_space;
    } else { // target能被分成两块空间
        front = target;
        target = (block_head *)((u64)target_space + size);
        target->size = front->size - size - sizeof(block_head);
        target->is_free == BLOCK_FREE;
        front->size = size;
    }
    
    // 设置双向链表
    target->next = front->next;
    if(target->next != NULL){
        target->next->prev = target;
    }
    target->prev = front;
    front->next = target;

    return target_space;
}




/**
 *  @brief: 将两块相邻空闲空间合并
 *  @param:
 *      bh1: 位于前面的block_head
 *      bh2: 位于后面的block_head
 *  @return: 
 */
void conbine_block_head(block_head *bh1, block_head *bh2) {
    // printk("[Test] free: bh1 = 0x%x, bh2 = 0x%x\n", (u64)bh1, (u64)bh2);
    bh1->size += bh2->size + sizeof(block_head); // 重新计算空间
     // 重新拼接链表
    bh1->next = bh2->next;
    if (bh2->next != NULL) {
        bh2->next->prev = bh1;
    }
    // 清空区域
    for(u64 i = 0; i < sizeof(block_head); i ++){
        *(u8 *)((u64)bh2 + i) = 0;
    }
}




/**
 *  @brief: 释放malloc的空间
 *  @param:
 *      free_space: 指向要释放的空间
 *  @return: 
 */
void free(void *free_space) {

    // 判空
    if(free_space == NULL){
        printk("[Error] free: free_space is NULL\n");
        return;
    }

    // 释放空间
    block_head *free_bh = (block_head *)((u64)free_space - sizeof(block_head));
    // printk("[Test] free: free_bh = 0x%x\n", (u64)free_bh);
    free_bh->is_free = BLOCK_FREE;

    // 合并前后相邻空间
    if(free_bh->prev != NULL && free_bh->prev->is_free == BLOCK_FREE){ // 前面相邻的节点free，拼接
        conbine_block_head(free_bh->prev, free_bh);
    }
    if(free_bh->next != NULL && free_bh->next->is_free == BLOCK_FREE){ // 后面相邻的节点free，拼接
        conbine_block_head(free_bh, free_bh->next);
    }
}




/**
 *  @brief: 获取mem大小
 *  @param:
 *      mem: 指向mem
 *  @return: mem大小
 */
u64 mem_get_size(void *mem){
    return ((block_head *)((u64)mem - sizeof(block_head)))->size;
}




/**
 *  @brief: 重新分配空间（此函数分配空间策略非最优解，有很大优化空间）
 *  @param:
 *      mem: 指向原空间
 *      size: 新空间的大小
 *  @return: 新空间指针
 */
void *realloc(void *mem, u64 size){
    void *ret = malloc(size);
    u8 *newMem = (u8 *)ret;
    u8 *oldMem = (u8 *)mem;
    for(u64 i = 0; i < mem_get_size(mem); i ++, oldMem ++, newMem ++){
        *newMem = *oldMem;
    }
    free(mem);
    return ret;
}
