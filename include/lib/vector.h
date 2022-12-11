
/**
 *  @author: Song XiuMing
 *  @date: 22/08/18
 */



#ifndef __VECTOR_H
#define __VECTOR_H


#include "type.h"


typedef struct vector{
    u64 *data;
    u64 used_size;
    u64 max_size;
}vector;



vector vector_init(u64 size);
u64 vector_get_index(vector vec, u64 index);
void vector_alter_index(vector vec, u64 index, u64 elem);
void vector_push(vector *vec, u64 elem);
u64 vector_pop(vector *vec);
u64 vector_find_elem(vector vec, u64 elem);


#endif /* __VECTOR_H */