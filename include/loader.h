
#ifndef __LOADER_H
#define __LOADER_H

#include "type.h"

void load_app( void );

u64 get_num_app( void );
u8* get_app_data(u64 app_id);

#endif /* __LOADER_H */
