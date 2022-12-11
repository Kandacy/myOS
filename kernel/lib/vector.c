
#include "type.h"
#include "lib/error.h"
#include "lib/vector.h"
#include "lib/stdlib.h"
#include "lib/string.h"




#if VECTOR_GENERICITY

/**
 *  @brief: vector初始化
 *  @param:
 *      v: 要初始化的vector
 *      elemSize: 一个数据的大小
 *      freefn: 单个元素的free函数
 *      copyfn: 单个元素copy函数
 *      cmpfn: 单个元素cmp函数
 *  @return: 
 *  @note: 普通变量只需要提供cmp函数，设计堆内存分配则需提供单元素cpy和free函数
 */
void vector_init(Vector* v, u64 elemSize, VectorFreeFunction freefn, VectorCopyFunction copyfn, VectorCompareFunction cmpfn){
    v->elemSize = elemSize;
    v->allocatedSize = 4;
    v->logicalSize = 0;
    v->data = malloc(v->elemSize * v->allocatedSize);
    assert(v->data!=NULL);
    v->freefn = freefn;
    v->cmpfn = cmpfn;
    v->copyfn = copyfn;
}




/**
 *  @brief: 销毁vector
 *  @param:
 *      v: 要销毁的vec
 *  @return: 
 */
void vector_destory(Vector* v){
    if (v->freefn!=NULL) {
        for (i64 i=0; i < v->logicalSize; i++) {
            void* elemAddr = (u8*)v->data + i*v->elemSize;
            v->freefn(elemAddr);
        }
    }
    free(v->data);     //如果没有析构函数，直接free掉data数据 
}




/**
 *  @brief: 获取vector中的元素
 *  @param:
 *      v: 要搞的vec
 *      position: 元素下标
 *  @return: 指向元素的指针
 */
void* vector_get(const Vector* v, u64 position){
    assert(position>=0);
    assert(position < v->logicalSize);
    return (u8*)v->data + position * v->elemSize;
}




/**
 *  @brief: 删除vector中的某个元素
 *  @param:
 *      v: 要搞的vec
 *      position: 要删除元素的位置
 *  @return: 
 */
void vector_delete(Vector* v, u64 position){
    assert(position>=0);
    assert(position < v->logicalSize);
    void* destAddr = (u8*)v->data+position * v->elemSize;
    if (v->freefn!=NULL)
        v->freefn(destAddr);
    i64 byteSize = (v->logicalSize-1-position) * v->elemSize;
    memmove(destAddr,(u8*)destAddr + v->elemSize, byteSize);
    v->logicalSize--;
}




/**
 *  @brief: 获取vector中的元素
 *  @param:
 *      v: 要搞的vec
 *      position: 元素下标
 *  @return: 指向元素的指针
 */
void* vector_pop(Vector *v){
    assert(v->logicalSize != 0);
    void* target = NULL;
    target = vector_get(v, v->logicalSize - 1);
    vector_delete(v, v->logicalSize - 1);
    return target;
}




static void vector_grow(Vector* v) {
    v->data = realloc(v->data,2*v->allocatedSize*v->elemSize);
    assert(v->data!=NULL);
    v->allocatedSize = 2*v->allocatedSize;
}
/**
 *  @brief: 向vector末尾插入元素
 *  @param:
 *      v: 要搞的vec
 *      elemAddr: 要插入元素的指针
 *  @return: 
 */
void vector_push(Vector* v, const void* elemAddr){
    if(v->logicalSize == v->allocatedSize)
        vector_grow(v);
    void* destAddr = (u8*)v->data + v->logicalSize * v->elemSize;
    /* memcpy(destAddr, elemAddr, v->elemSize); */
	if(v->copyfn != NULL)
		v->copyfn(destAddr, elemAddr);
    memcpy(destAddr,elemAddr,v->elemSize);
    v->logicalSize++;
}




/**
 *  @brief: 获取vector大小
 *  @param:
 *      v: 要搞的vec
 *  @return: vector的大小
 */
u64 vector_size(const Vector* v){
    return v->logicalSize;
}




/**
 *  @brief: 查找vector内某元素首次出现位置
 *  @param:
 *      v: 要搞的vec
 *      key: 要查元素的地址
 *      startIndex: 开始查找的位置
 *  @return: 位置下标（查找失败则返回-1）
 */
i64 vector_find_elem(Vector* v, const void* key, u64 startIndex){
    assert(v->cmpfn!=NULL);
    assert(startIndex >= 0);
    assert(startIndex <= v->logicalSize);
    void* base = (u8*)v->data + startIndex * v->elemSize;
    i64 MAX = v->logicalSize - startIndex;
    i64 flag = -1;
    for (i64 i=0 ; i<MAX; i++) {
        void* elemAddr = (u8*)base + i*v->elemSize;
        if (0 == v->cmpfn(elemAddr,key)) {
            flag = i+startIndex;
            return flag;
        }
    }
    return flag;
}


#else

/**
 *  @brief: vector初始化
 *  @param:
 *      vec: 要初始化的vector
 *      size: 要开的大小
 *      data_size: 一个数据的大小
 *  @return: 
 */
void vector_init(vector *vec, u64 size, u64 data_size){
    vec->data = (void *)malloc(size * data_size);
    vec->data_size = data_size;
    vec->max_size = size;
    vec->used_size = 0;
    return vec;
}




/**
 *  @brief: vector获取数组内元素位置
 *  @param:
 *      vec: 要搞的vec
 *      index: index
 *  @return: 指向元素位置的指针
 */
void *vector_get_index(vector *vec, u64 index){
    return (void *)((u8 *)vec->data + index * vec->data_size);
}




/**
 *  @brief: 替换vector中index位置的元素
 *  @param:
 *      vec: 要搞的vec
 *      index: index
 *      elem: 要替换的元素
 *  @return: vector实例
 */
void vector_replace_index(vector *vec, u64 index, void *elem){
    for(u64 i = 0; i < vec->data_size; i ++){
        
    }
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


#endif


