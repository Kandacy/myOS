
#ifndef __VECTOR_H
#define __VECTOR_H


#include "type.h"


// vector是否使用泛型实现
#define VECTOR_GENERICITY  1


#if VECTOR_GENERICITY

typedef i64 (*VectorCompareFunction)(const void* elemAddr1, const void* elemAddr2);
typedef void (*VectorCopyFunction)(void* destAddr, const void* srcAddr);
typedef void (*VectorFreeFunction)(void* elemAddr);

typedef struct {
    void* data;                       //指向数据元素的指针
    u64 elemSize;                     //每个元素占内存的大小
    u64 allocatedSize;                //已分配内存元素的大小
    u64 logicalSize;                  //实际储存元素的个数
    VectorFreeFunction freefn;      //设置了一个free函数，根据数据地址释放不定长部分的内存空间
    VectorCopyFunction copyfn;      //提供源数据和目的数据的地址，为目的数据中不定长部分申请内存空间，将源数据中不定长部分拷贝到目的数据中不定长部分
    VectorCompareFunction cmpfn;    //根据参数地址比较两数据的大小
}Vector;

void vector_init(Vector* v, u64 elemSize, VectorFreeFunction freefn, VectorCopyFunction copyfn, VectorCompareFunction cmpfn);
void vector_destory(Vector* v);
void* vector_get(const Vector* v, u64 position);
void vector_delete(Vector* v, u64 position);
void* vector_pop(Vector *v);
void vector_push(Vector* v, const void* elemAddr);
u64 vector_size(const Vector* v);
i64 vector_find_elem(Vector* v, const void* key, u64 startIndex);



/* 示例 */
#if 0
//整型比较函数
i64 INT_cmp(const void* elemAddr1,const void* elemAddr2) {
    return *(int*)elemAddr1 - *(int*)elemAddr2;
}
//浮点数比较函数,以double为例
i64 FLOAT_cmp(const void* elemAddr1,const void* elemAddr2) {
    return *(double*)elemAddr1 - *(double*)elemAddr2;
}
//字符型比较函数
i64 STR_cmp(const void* elemAddr1,const void* elemAddr2) {
    return strcmp(*(char**)elemAddr1,*(char**)elemAddr2);
}
//字符串的析构函数
void STR_free(void* elemAddr) {
    free(*(char**)elemAddr);
}
//字符串的复制函数
void STR_copy(void* destAddr,const void* srcAddr) {
    *(char**)destAddr = strdup(*(char**)srcAddr);
}
#endif  /* 示例 */


#else

typedef struct vector{
    void *data;
    u64 data_size;
    u64 used_size;
    u64 max_size;
}vector;

vector vector_init(u64 size);
u64 vector_get_index(vector vec, u64 index);
void vector_alter_index(vector vec, u64 index, u64 elem);
void vector_push(vector *vec, u64 elem);
u64 vector_pop(vector *vec);
u64 vector_find_elem(vector vec, u64 elem);

#endif


#endif /* __VECTOR_H */