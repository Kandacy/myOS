
#ifndef __TASK_H
#define __TASK_H

#include "type.h"
#include "trap/trap.h"


void task_manager_init( void );
void run_first_app( void );

void exit_current_app( void );
void suspend_current_app( void );

u64 get_current_app_id( void );
u64 get_current_user_satp( void );
TrapContext *get_current_trap_cx( void );

#endif /* __TASK_H */
