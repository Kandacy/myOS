
#include "task/task.h"
#include "type.h"
#include "lib/error.h"
#include "trap/trap.h"
#include "mm/mem.h"
#include "config.h"
#include "driver/riscv.h"
#include "lib/stdio.h"



/* task status */
#define TSTATUS_UNINIT   0
#define TSTATUS_READY    1
#define TSTATUS_RUNNING  2
#define TSTATUS_EXITED   3



/**
 *  切换任务时保存的上下文
 */
typedef struct TaskContext{
    u64 ra;
    u64 sp;
    u64 s_regs[12];
}TaskContext;


/**
 *  TCB
 */
typedef struct TaskControlBlock{
    u64 status;
    TaskContext tc;
}TaskControlBlock;


/**
 *  任务管理器，全局仅有一个
 */
static struct {
    u64 num_app;
    u64 current_app;
    TaskControlBlock tcb[APP_NUM_MAX];
}TASK_MANAGER;



extern void __switch(TaskContext *tc1, TaskContext *tc2);



/**
 *  @brief: 初始化TaskManager，初始化各app的kernel_stack
 *  @param:
 *  @return:
 */
void task_manager_init( void ){

    extern void __restore(TrapContext *cx);
    printk("[Test] __restore: 0x%x\n", (u64)__restore);
    TASK_MANAGER.num_app = APP_NUM_MAX;
    TASK_MANAGER.current_app = 0;

    /* 初始化TCB */
    for(u64 i = 0; i < APP_NUM_MAX; i ++){

        // 获取kernel_stack压入TrapContext后的地址
        u64 target_sp = get_kernel_stack_top(i) - sizeof(TrapContext);

        TASK_MANAGER.tcb[i].status = TSTATUS_READY; // 设置app状态
        TASK_MANAGER.tcb[i].tc.ra = (u64)__restore; // 设置switch函数返回地址
        TASK_MANAGER.tcb[i].tc.sp = target_sp; // 设置各自kernel_stack位置

        /* 初始化kernel_stack */
        TrapContext tc = {
            {0},
            0,
            0
        };
        tc.sepc = APP_BASE_ADDRESS + (APP_MAX_SIZE * i); // 设置app首次进入的地址
        tc.sstatus = r_sstatus() & (~SSTATUS_SPP); // 设置sstatus寄存器为内核状态
        tc.x_regs[2] = get_user_stack_top(i); // 保证sscratch指向user_stack

        // 将TrapContext压入kernel_stack
        *(TrapContext *)target_sp = tc;
    }
}




/**
 *  @brief: 执行下一个ready的app
 *  @param:
 *      status: 当前app要设置的状态
 *  @return:
 */
void run_next_app( u64 status ) {

    /* 寻找ready的app */
    u64 target_app_num;
    for(target_app_num = (TASK_MANAGER.current_app + 1) % APP_NUM_MAX; target_app_num != TASK_MANAGER.current_app; target_app_num ++){
        if( TASK_MANAGER.tcb[target_app_num].status == TSTATUS_READY ){
            break;
        }
    }
    // 没有ready的应用
    if((target_app_num == TASK_MANAGER.current_app) && (TASK_MANAGER.tcb[target_app_num].status != TSTATUS_READY)){
        panic("no app is ready");
    }

    TaskControlBlock *target_app_tcb = &TASK_MANAGER.tcb[target_app_num];
    TaskControlBlock *current_app_tcb = &TASK_MANAGER.tcb[TASK_MANAGER.current_app];

    /* 改变状态 */
    current_app_tcb->status = status;
    target_app_tcb->status = TSTATUS_RUNNING;
    TASK_MANAGER.current_app = target_app_num;

    /* 交换TaskContext */
    __switch(&current_app_tcb->tc, &target_app_tcb->tc);
}




/**
 *  @brief: 执行首个app
 *  @param:
 *  @return:
 */
void run_first_app( void ) {

    static TaskContext temp;

    /* 改变状态 */
    TASK_MANAGER.tcb[TASK_MANAGER.current_app].status = TSTATUS_RUNNING;
    printk("[kernel] ready to run fitst app\n");

    /* 交换TaskContext */
    __switch(&temp, &TASK_MANAGER.tcb[TASK_MANAGER.current_app].tc);

    panic("run first app failed");
}




/**
 *  @brief: 退出当前app
 *  @param:
 *  @return:
 */
void exit_current_app( void ){
    run_next_app(TSTATUS_EXITED);
}




/**
 *  @brief: 挂起当前app
 *  @param:
 *  @return:
 */
void suspend_current_app( void ){
    run_next_app(TSTATUS_READY);
}
