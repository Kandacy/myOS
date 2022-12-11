
#ifndef __CONFIG_H
#define __CONFIG_H


/* task */
#define APP_NUM_MAX 3
#define APP_BASE_ADDRESS  0x80400000
#define APP_MAX_SIZE      0x20000


/* system */
// 时钟频率
#define CLOCK_FREQ      24000000
// 任务频率
#define TICKS_PER_SEC   50


/* mm */
// 物理内存中止地址
#define MEMORY_END  0x80800000

/* alloc */
// 用于malloc的堆内存
#define KERNEL_HEAP_SIZE 0x100000



#endif /* __CONFIG_H */
