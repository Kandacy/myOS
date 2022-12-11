
/**
 *  @author: Song XiuMing
 *  @date: 22/08/17
 */



#ifndef __STDLIB_H
#define __STDLIB_H


#include "type.h"


void kernel_heap_init( void );

void *malloc(u64 size);
void free(void *free_space);
void *realloc(void *mem, u64 size);


#endif /* __STDLIB_H */