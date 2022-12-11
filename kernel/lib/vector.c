
#include "type.h"
#include "lib/vector.h"
#include "lib/stdlib.h"




/**
 *  @brief: vector初始化
 *  @param:
 *      size: 要开的大小
 *  @return: vector实例
 */
vector vector_init(u64 size){
    vector vec;
    vec.data = (u64 *)malloc(size * sizeof(u64));
    vec.used_size = 0;
    vec.max_size = size;
    return vec;
}




/**
 *  @brief: vector获取数组内元素
 *  @param:
 *      vec: 要搞的vec
 *      index: index
 *  @return: vector实例
 */
u64 vector_get_index(vector vec, u64 index){
    return vec.data[index];
}




/**
 *  @brief: 替换vector中index位置的元素
 *  @param:
 *      vec: 要搞的vec
 *      index: index
 *      elem: 要替换的元素
 *  @return: vector实例
 */
void vector_alter_index(vector vec, u64 index, u64 elem){
    vec.data[index] = elem;
}




/**
 *  @brief: 向vector末尾插入元素
 *  @param:
 *      vec: 要搞的vec
 *      elem: 要插入的元素
 *  @return: vector实例
 */
void vector_push(vector *vec, u64 elem){
    if(vec->used_size == vec->max_size){
        vec->data = (u64 *)realloc(vec->data, (vec->max_size + 10) * sizeof(u64));
        vec->max_size += 10;
    }
    vec->data[vec->used_size ++] = elem;
}




/**
 *  @brief: 向vector末尾插入元素
 *  @param:
 *      vec: 要搞的vec
 *      elem: 要插入的元素
 *  @return: vector实例
 */
u64 vector_pop(vector *vec){
    return vec->data[-- vec->used_size];
}




/**
 *  @brief: 查找vector内某元素首次出现位置
 *  @param:
 *      vec: 要搞的vec
 *      elem: 要查的元素
 *  @return: 位置下标（查找失败则返回used_size）
 */
u64 vector_find_elem(vector vec, u64 elem){
    for(u64 i = 0; i < vec.used_size; i ++){
        if(vec.data[i] == elem){
            return i;
        }
    }
    return vec.used_size; // 查找失败
}


