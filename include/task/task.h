
#ifndef __TASK_H
#define __TASK_H

#include "type.h"


void task_manager_init( void );
void run_first_app( void );

void exit_current_app( void );
void suspend_current_app( void );


#endif /* __TASK_H */
